// Copyright Zero Games. All Rights Reserved.

#include "Misc/ZUserWidgetLruCache.h"

#include "Blueprint/UserWidget.h"

UUserWidget* UZUserWidgetLruCache::GetActiveWidget(FName id) const
{
	const TObjectPtr<UUserWidget>* widget = ActiveWidgetMap.Find(id);
	return widget && ::IsValid(widget->Get()) ? widget->Get() : nullptr;
}

UUserWidget* UZUserWidgetLruCache::GetInactiveWidget(FName id) const
{
	const FZUserWidgetLruCacheRec* rec = InactiveWidgetMap.Find(id);
	return rec ? rec->Widget : nullptr;
}

void UZUserWidgetLruCache::ActivateNewWidget(FName id, UUserWidget* widget)
{
	ActiveWidgetMap.Emplace(id, widget);
}

UUserWidget* UZUserWidgetLruCache::ActivateExistingWidget(FName id)
{
	FZUserWidgetLruCacheRec rec;
	if (InactiveWidgetMap.RemoveAndCopyValue(id, rec))
	{
		ActiveWidgetMap.Emplace(id, rec.Widget);
	}

	return rec.Widget;
}

UUserWidget* UZUserWidgetLruCache::DeactivateExistingWidget(FName id, bool& released)
{
	released = false;
	
	TObjectPtr<UUserWidget> widget;
	if (!ActiveWidgetMap.RemoveAndCopyValue(id, widget) || !::IsValid(widget))
	{
		return nullptr;
	}

	if (MaxInactiveCount < 1)
	{
		released = true;
	}
	else
	{
		InactiveWidgetMap.Emplace(id, FZUserWidgetLruCacheRec { widget, FPlatformTime::Seconds() });
		if (InactiveWidgetMap.Num() > MaxInactiveCount)
		{
			Trim();
		}

		if (!TimerDelegate.IsValid())
		{
			TimerDelegate = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](float deltaTime){ Trim(); return true; }), 10);
		}
	}

	return widget;
}

void UZUserWidgetLruCache::ReleaseAllWidgets(TFunctionRef<void(UUserWidget*, bool)> callback)
{
	TMap<FName, FZUserWidgetLruCacheRec> inactive = MoveTemp(InactiveWidgetMap);
	TMap<FName, TObjectPtr<UUserWidget>> active = MoveTemp(ActiveWidgetMap);
	
	InactiveWidgetMap.Empty();
	ActiveWidgetMap.Empty();
	
	for (const auto& [_, rec] : inactive)
	{
		callback(rec.Widget, false);
	}
	
	for (const auto& [_, widget] : active)
	{
		callback(widget, true);
	}
}

void UZUserWidgetLruCache::ForeachActiveWidget(TFunctionRef<void(UUserWidget*)> action) const
{
	for (const auto& [_, widget] : ActiveWidgetMap)
	{
		action(widget);
	}
}

void UZUserWidgetLruCache::Resetting()
{
	if (TimerDelegate.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TimerDelegate);
		TimerDelegate.Reset();
	}
	
	Super::Resetting();
}

void UZUserWidgetLruCache::Trim()
{
	double now = FPlatformTime::Seconds();
	TMap<FName, FZUserWidgetLruCacheRec> inactive = MoveTemp(InactiveWidgetMap);
	InactiveWidgetMap.Empty();
	for (const auto& [id, rec] : inactive)
	{
		if (now - rec.Timestamp < MaxAliveSeconds)
		{
			InactiveWidgetMap.Emplace(id, rec);
		}
		else
		{
			NotifyReleasingInactive(rec.Widget);
		}
	}
}
