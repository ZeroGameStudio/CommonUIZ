// Copyright Zero Games. All Rights Reserved.

#include "ZUserWidgetPool.h"

#include "Blueprint/UserWidget.h"

void UZUserWidgetPoolLibrary::Return(TScriptInterface<IZPoolableUserWidget> widget)
{
	auto w = Cast<UUserWidget>(widget.GetObject());
	if (!w)
	{
		return;
	}
	
	UZUserWidgetPool* pool = IZPoolableUserWidget::Execute_GetPool(w);
	if (!pool)
	{
		return;
	}
	
	pool->Return(w);
}

UZUserWidgetPool* UZUserWidgetPool::Create(UWidget* owner, TSubclassOf<UUserWidget> pooledWidgetClass, int32 capacity, int32 precacheCount, EZUserWidgetPoolExceedPolicy exceedPolicy)
{
	if (!pooledWidgetClass)
	{
		return nullptr;
	}
	
	if (!pooledWidgetClass->ImplementsInterface(UZPoolableUserWidget::StaticClass()))
	{
		return nullptr;
	}
	
	auto pool = NewObject<ThisClass>(owner);
	
	pool->PooledWidgetClass = pooledWidgetClass;
	pool->Capacity = FMath::Max(capacity, 1);
	pool->PrecacheCount = FMath::Min(precacheCount, pool->Capacity);
	pool->ExceedPolicy = exceedPolicy;
	
	pool->Initialize();
	
	return pool;
}

UZUserWidgetPool::UZUserWidgetPool()
{
	if (!IsTemplate())
	{
		Owner = CastChecked<UWidget>(GetOuter());
	}
}

UUserWidget* UZUserWidgetPool::Rent()
{
	UUserWidget* widget = nullptr;
	while (CachedWidgets.Num() > 0)
	{
		widget = CachedWidgets.Pop();
		if (widget)
		{
			break;
		}
	}
	
	if (!widget)
	{
		ensureAlways(CachedWidgets.Num() == 0);
		if (RentedWidgets.Num() < Capacity)
		{
			widget = Create();
		}
		else if (ExceedPolicy == EZUserWidgetPoolExceedPolicy::Create)
		{
			widget = Create();
		}
		else if (ExceedPolicy == EZUserWidgetPoolExceedPolicy::Preempt)
		{
			widget = Preempt();
		}
	}
	
	/*
	 * 两种正常情况会导致借不出：
	 * 1. 超限策为 Preempt，但池中控件不支持 Preempt。
	 * 2. 超限策略为 ReturnNull。
	 */
	if (!widget)
	{
		ensureAlways(CachedWidgets.Num() + RentedWidgets.Num() >= Capacity);
		ensureAlways(ExceedPolicy != EZUserWidgetPoolExceedPolicy::Create);
		return nullptr;
	}
	
	IZPoolableUserWidget::Execute_PoolableUserWidget_Initialize(widget);
	ensureAlways(!RentedWidgets.Contains(widget));
	RentedWidgets.Add(widget);
	return widget;
}

void UZUserWidgetPool::Initialize()
{
	int32 precacheCount = FMath::Min(PrecacheCount, Capacity);
	for (int32 i = 0; i < precacheCount; ++i)
	{
		UUserWidget* widget = Create();
		if (!widget)
		{
			break;
		}
		
		CachedWidgets.Add(widget);
	}
	
	ensureAlways(CachedWidgets.Num() < Capacity);
}

UUserWidget* UZUserWidgetPool::Create()
{
	UClass* cls = PooledWidgetClass.Get();
	if (!cls)
	{
		return nullptr;
	}
	
	if (!cls->ImplementsInterface(UZPoolableUserWidget::StaticClass()))
	{
		return nullptr;
	}
	
	if (!Owner)
	{
		return nullptr;
	}
	
	UUserWidget* widget = ::CreateWidget(Owner, cls);
	IZPoolableUserWidget::Execute_SetPool(widget, this);
	if (!ensureAlways(IZPoolableUserWidget::Execute_GetPool(widget) == this))
	{
		widget->MarkAsGarbage();
		return nullptr;
	}
	
	return widget;
}

UUserWidget* UZUserWidgetPool::Preempt()
{
	UClass* cls = PooledWidgetClass.Get();
	if (!cls)
	{
		return nullptr;
	}
	
	if (!cls->ImplementsInterface(UZPoolableUserWidget::StaticClass()))
	{
		return nullptr;
	}
	
	if (!IZPoolableUserWidget::Execute_PoolableUserWidget_AllowsPreempt(cls->GetDefaultObject()))
	{
		return nullptr;
	}
	
	while (RentedWidgets.Num() > 0)
	{
		UUserWidget* widget = RentedWidgets[0];
		RentedWidgets.RemoveAt(0);
		if (!widget)
		{
			continue;
		}
		
		IZPoolableUserWidget::Execute_PoolableUserWidget_Uninitialize(widget, true);
		
		// 此时 widget 的状态应该等价于刚创建的（不在容器内，未初始化）。
		return widget;
	}
	
	return nullptr;
}

void UZUserWidgetPool::Return(UUserWidget* widget)
{
	if (!ensureAlways(IZPoolableUserWidget::Execute_GetPool(widget) == this))
	{
		return;
	}
	
	// 重复归还被认为是必须修复的错误，因为会导致控件复用后的行为不可预期。
	ensureAlways(RentedWidgets.Contains(widget));
	ensureAlways(!CachedWidgets.Contains(widget));
	
	IZPoolableUserWidget::Execute_PoolableUserWidget_Uninitialize(widget, false);
	RentedWidgets.Remove(widget);
	if (CachedWidgets.Num() + RentedWidgets.Num() < Capacity)
	{
		CachedWidgets.Add(widget);
	}
}


