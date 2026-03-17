// Copyright Zero Games. All Rights Reserved.

#include "Misc/ZUserWidgetCacheBaseInterface.h"

#include "CommonUIZRuntimeLogChannels.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

void UZUserWidgetCacheBaseInterface::AddReferencedObjects(UObject* self, FReferenceCollector& collector)
{
	Super::AddReferencedObjects(self, collector);

	collector.AddStableReferenceMap(static_cast<ThisClass*>(self)->CachedSlateWidgets);
}

void UZUserWidgetCacheBaseInterface::DeactivateWidget(FName id, bool releaseSlateWidget)
{
	bool released = false;
	if (UUserWidget* deactivatedWidget = DeactivateExistingWidget(id, released))
	{
		if (releaseSlateWidget || released)
		{
			CachedSlateWidgets.Remove(deactivatedWidget);
		}
		
		WidgetDeactivatedDelegate.Broadcast(deactivatedWidget);
		if (released)
		{
			WidgetReleasingDelegate.Broadcast(deactivatedWidget);
		}
	}
}

void UZUserWidgetCacheBaseInterface::RebuildWidgets()
{
	ForeachActiveWidget([this](UUserWidget* widget)
	{
		CachedSlateWidgets.Add(widget, widget->TakeWidget());
	});
}

void UZUserWidgetCacheBaseInterface::Reset()
{
	Resetting();
	
	CachedSlateWidgets.Empty();
	
	ReleaseAllWidgets([this](UUserWidget* widget, bool active)
	{
		if (active)
		{
			WidgetDeactivatedDelegate.Broadcast(widget);
		}
		WidgetReleasingDelegate.Broadcast(widget);
	});
}

void UZUserWidgetCacheBaseInterface::BeginDestroy()
{
	if (!IsTemplate())
	{
		Reset();
	}
	
	UObject::BeginDestroy();
}

void UZUserWidgetCacheBaseInterface::NotifyReleasingInactive(UUserWidget* widget)
{
	CachedSlateWidgets.Remove(widget);
	WidgetReleasingDelegate.Broadcast(widget);
}

UUserWidget* UZUserWidgetCacheBaseInterface::InternalActivateWidget(FName id, TSubclassOf<UUserWidget> widgetClass, bool& newlyCreated)
{
	newlyCreated = false;
	
	if (!widgetClass)
	{
		return nullptr;
	}
	
	if (UUserWidget* activeWidget = GetActiveWidget(id))
	{
		check(activeWidget->GetClass() == widgetClass);
		return activeWidget;
	}
	
	if (UUserWidget* activatedWidget = ActivateExistingWidget(id))
	{
		check(activatedWidget->GetClass() == widgetClass);
		WidgetActivatedDelegate.Broadcast(activatedWidget);
		return activatedWidget;
	}
	
	UUserWidget* widget = CreateWidget(widgetClass);
	if (widget)
	{
		newlyCreated = true;
		
		ActivateNewWidget(id, widget);
		
		WidgetCreatedDelegate.Broadcast(widget);
		WidgetActivatedDelegate.Broadcast(widget);
	}

	return widget;
}

UUserWidget* UZUserWidgetCacheBaseInterface::CreateWidget(TSubclassOf<UUserWidget> widgetClass)
{
	UUserWidget* widget;
	bool cacheSlateWidget = true;
	UObject* outer = GetOuter();
	if (auto outerPlayerController = Cast<APlayerController>(outer))
	{
		widget = ::CreateWidget(outerPlayerController, widgetClass);
	}
	else if (auto outerWidget = Cast<UWidget>(outer))
	{
		widget = ::CreateWidget(outerWidget, widgetClass);
		cacheSlateWidget = outerWidget->GetCachedWidget().IsValid();
	}
	else if (auto outerWidgetTree = Cast<UWidgetTree>(outer))
	{
		widget = ::CreateWidget(outerWidgetTree, widgetClass);
	}
	else if (auto outerGameInstance = Cast<UGameInstance>(outer))
	{
		widget = ::CreateWidget(outerGameInstance, widgetClass);
	}
	else if (auto outerWorld = Cast<UWorld>(outer))
	{
		widget = ::CreateWidget(outerWorld, widgetClass);
	}
	else
	{
		UE_LOG(LogCommonUIZRuntime, Error, TEXT("User Widget Cache has an unknown outer type [%s]."), *GetNameSafe(outer->GetClass()));
		widget = nullptr;
	}

	if (widget && cacheSlateWidget)
	{
		TSharedPtr<SWidget>& cached = CachedSlateWidgets.FindOrAdd(widget);
		if (!cached.IsValid())
		{
			cached = widget->TakeWidget();
		}
	}

	return widget;
}


