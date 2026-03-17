// Copyright Zero Games. All Rights Reserved.

#include "Widget/ZActivatableWidgetContainer.h"

#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "Misc/ZUserWidgetLruCache.h"

UZActivatableWidgetContainer::UZActivatableWidgetContainer()
	: CacheClass(UZUserWidgetLruCache::StaticClass())
{
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

UCommonActivatableWidget* UZActivatableWidgetContainer::PushWidget(FName id, TSubclassOf<UCommonActivatableWidget> widgetClass, bool& newlyCreated)
{
	if (auto widget = EnsureCache().ActivateWidget<UCommonActivatableWidget>(id, widgetClass, newlyCreated))
	{
		if (widget != GetActiveWidget())
		{
			InternalAddWidget(id, widget);
		}

		return widget;
	}

	return nullptr;
}

void UZActivatableWidgetContainer::PopWidget(int32 times)
{
	for (int32 i = 0; i < times; ++i)
	{
		if (SortedWidgets.IsEmpty())
		{
			break;
		}
	
		InternalRemoveWidget(SortedWidgets.Last());
	}
}

void UZActivatableWidgetContainer::RemoveWidget(FName id)
{
	InternalRemoveWidget(id);
}

void UZActivatableWidgetContainer::RemoveWidgetInstance(UCommonActivatableWidget* widget)
{
	for (const auto& pair : Widgets)
	{
		if (pair.Value == widget)
		{
			InternalRemoveWidget(pair.Key);
			break;
		}
	}
}

void UZActivatableWidgetContainer::ClearWidgets()
{
	PopWidget(GetNumWidgets());
}

void UZActivatableWidgetContainer::Reset()
{
	ClearWidgets();
	if (Cache)
	{
		Cache->Reset();
	}
}

UCommonActivatableWidget* UZActivatableWidgetContainer::GetActiveWidget() const
{
	FName dummy;
	return GetActiveWidgetWithId(dummy);
}

UCommonActivatableWidget* UZActivatableWidgetContainer::GetActiveWidgetWithId(FName& id) const
{
	if (SortedWidgets.IsEmpty())
	{
		id = NAME_None;
		return nullptr;
	}
	
	id = SortedWidgets.Last();
	return Widgets[id];
}

UCommonActivatableWidget* UZActivatableWidgetContainer::GetWidget(FName id) const
{
	const TObjectPtr<UCommonActivatableWidget>* widget = Widgets.Find(id);
	return widget && ::IsValid(widget->Get()) ? widget->Get() : nullptr;
}

int32 UZActivatableWidgetContainer::GetNumWidgets() const
{
	return Widgets.Num();
}

void UZActivatableWidgetContainer::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (!Cache || Cache->GetClass() != CacheClass)
		{
			if (Cache)
			{
				Cache->Reset();
				Cache = nullptr;
			}
		}
		
		UClass* backgroundClass = Background ? Background->GetClass() : nullptr;
		if (backgroundClass != BackgroundClass)
		{
			if (Background)
			{
				if (TSharedPtr<SWidget> widget = Background->GetCachedWidget(); ensure(widget))
				{
					MyBackgroundOverlay->RemoveSlot(widget.ToSharedRef());
				}
				Background = nullptr;
			}
		}
	}
#endif

	(void)EnsureCache();

	if (!Background && BackgroundClass)
	{
		Background = ::CreateWidget(this, BackgroundClass);
		MyBackgroundOverlay->AddSlot() [ Background->TakeWidget() ];
	}
}

TSharedRef<SWidget> UZActivatableWidgetContainer::RebuildWidget()
{
	MyOverlay = SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SAssignNew(MyBackgroundOverlay, SOverlay)
		];

	return MyOverlay.ToSharedRef();
}

void UZActivatableWidgetContainer::ReleaseSlateResources(bool bReleaseChildren)
{
	Reset();
	
	MyBackgroundOverlay.Reset();
	MyOverlay.Reset();
	Background = nullptr;

	Super::ReleaseSlateResources(bReleaseChildren);
}

int32 UZActivatableWidgetContainer::GetDesiredActiveWidgetIndex() const
{
	return Widgets.Num() - 1;
}

void UZActivatableWidgetContainer::InternalAddWidget(FName id, UCommonActivatableWidget* widget)
{
	if (UCommonActivatableWidget* prevActiveWidget = GetActiveWidget())
	{
		InternalDeactivateWidget(prevActiveWidget);
	}
	
	InternalRemoveWidgetFromList(id);
	InternalAddWidgetToList(id, widget);
	InternalActivateWidget(widget);
	
	RefreshBackgroundVisibility();
}

void UZActivatableWidgetContainer::InternalRemoveWidget(FName id)
{
	FName prevActiveId;
	UCommonActivatableWidget* prevActiveWidget = GetActiveWidgetWithId(prevActiveId);
	if (id == prevActiveId)
	{
		InternalDeactivateWidget(prevActiveWidget);
	}
	
	InternalRemoveWidgetFromList(id);
	EnsureCache().DeactivateWidget(id);

	UCommonActivatableWidget* curActiveWidget = GetActiveWidget();
	if (curActiveWidget && prevActiveWidget != curActiveWidget)
	{
		InternalActivateWidget(curActiveWidget);
	}

	RefreshBackgroundVisibility();
}

void UZActivatableWidgetContainer::InternalAddWidgetToList(FName id, UCommonActivatableWidget* widget)
{
	Widgets.Emplace(id, widget);
	SortedWidgets.Emplace(id);
	
	MyOverlay->AddSlot() [ widget->TakeWidget() ];
	
	check(Widgets.Num() == SortedWidgets.Num());
}

void UZActivatableWidgetContainer::InternalRemoveWidgetFromList(FName id)
{
	TObjectPtr<UCommonActivatableWidget> widget;
	if (Widgets.RemoveAndCopyValue(id, widget))
	{
		verify(SortedWidgets.Remove(id));
		if (TSharedPtr<SWidget> swidget = widget->GetCachedWidget())
		{
			MyOverlay->RemoveSlot(swidget.ToSharedRef());
		}
	}
	
	check(Widgets.Num() == SortedWidgets.Num());
}

void UZActivatableWidgetContainer::InternalActivateWidget(UCommonActivatableWidget* widget)
{
	ensure(widget == GetActiveWidget());
	ensure(!widget->IsActivated());
	
	widget->OnDeactivated().AddUObject(this, &ThisClass::HandleWidgetDeactivated, widget);
	widget->ActivateWidget();
}

void UZActivatableWidgetContainer::InternalDeactivateWidget(UCommonActivatableWidget* widget)
{
	widget->OnDeactivated().RemoveAll(this);
	widget->DeactivateWidget();
}

void UZActivatableWidgetContainer::RefreshBackgroundVisibility()
{
	if (Background)
	{
		Background->SetVisibility(!!Widgets.Num() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

UZUserWidgetCacheBaseInterface& UZActivatableWidgetContainer::EnsureCache()
{
	if (!Cache)
	{
		Cache = NewObject<UZUserWidgetCacheBaseInterface>(this, CacheClass);
		Cache->OnWidgetReleasing().AddWeakLambda(this, [this](UUserWidget* widget){ OnWidgetReleasing.Broadcast(CastChecked<UCommonActivatableWidget>(widget)); });
	}

	return *Cache;
}

void UZActivatableWidgetContainer::HandleWidgetDeactivated(UCommonActivatableWidget* widget)
{
	FName id;
	ensure(widget == GetActiveWidgetWithId(id));
	InternalRemoveWidget(id);
}


