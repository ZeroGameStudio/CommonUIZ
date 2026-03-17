// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZUserWidgetCacheBaseInterface.generated.h"

class UWidget;

UCLASS(Abstract, Transient, DefaultToInstanced, Blueprintable)
class COMMONUIZRUNTIME_API UZUserWidgetCacheBaseInterface : public UObject
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FWidgetLifecycleEvent, UUserWidget*);

public:
	static void AddReferencedObjects(UObject* self, FReferenceCollector& collector);

public:
	template <typename T = UUserWidget>
	requires std::is_base_of_v<UUserWidget, T>
	T* ActivateWidget(FName id, TSubclassOf<T> widgetClass, bool& newlyCreated)
	{
		return static_cast<T*>(InternalActivateWidget(id, widgetClass, newlyCreated));
	}
	
	template <typename T = UUserWidget>
	requires std::is_base_of_v<UUserWidget, T>
	T* ActivateWidget(FName id, TSubclassOf<T> widgetClass = T::StaticClass())
	{
		bool dummy;
		return ActivateWidget<T>(id, widgetClass, dummy);
	}
	
	template <typename T = UUserWidget>
	requires std::is_base_of_v<UUserWidget, T>
	T* ActivateWidget(FName id, bool& newlyCreated)
	{
		return ActivateWidget<T>(id, T::StaticClass(), newlyCreated);
	}
	
	void DeactivateWidget(FName id, bool releaseSlateWidget = false);
	void RebuildWidgets();
	void Reset();

public:
	FWidgetLifecycleEvent::RegistrationType& OnWidgetCreated() { return WidgetCreatedDelegate; }
	FWidgetLifecycleEvent::RegistrationType& OnWidgetActivated() { return WidgetActivatedDelegate; }
	FWidgetLifecycleEvent::RegistrationType& OnWidgetDeactivated() { return WidgetDeactivatedDelegate; }
	FWidgetLifecycleEvent::RegistrationType& OnWidgetReleasing() { return WidgetReleasingDelegate; }

protected:
	virtual void BeginDestroy() override;
	
protected:
	virtual UUserWidget* GetActiveWidget(FName id) const { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::GetActiveWidget); return nullptr; }
	virtual UUserWidget* GetInactiveWidget(FName id) const { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::GetInactiveWidget); return nullptr; }

	virtual void ActivateNewWidget(FName id, UUserWidget* widget) { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::ActivateNewWidget); }
	virtual UUserWidget* ActivateExistingWidget(FName id) { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::ActivateExistingWidget); return nullptr; }
	virtual UUserWidget* DeactivateExistingWidget(FName id, bool& released) { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::DeactivateExistingWidget); return nullptr; }

	virtual void ReleaseAllWidgets(TFunctionRef<void(UUserWidget*, bool)> callback) { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::ReleaseAllWidgets); }
	virtual void ForeachActiveWidget(TFunctionRef<void(UUserWidget*)> action) const { PURE_VIRTUAL(UZUserWidgetCacheBaseInterface::ForeachActiveWidget); }

	virtual void Resetting(){}

protected:
	void NotifyReleasingInactive(UUserWidget* widget);
	
private:
	UUserWidget* InternalActivateWidget(FName id, TSubclassOf<UUserWidget> widgetClass, bool& newlyCreated);
	UUserWidget* CreateWidget(TSubclassOf<UUserWidget> widgetClass);

private:
	TMap<TObjectPtr<UUserWidget>, TSharedPtr<SWidget>> CachedSlateWidgets;

private:
	FWidgetLifecycleEvent WidgetCreatedDelegate;
	FWidgetLifecycleEvent WidgetActivatedDelegate;
	FWidgetLifecycleEvent WidgetDeactivatedDelegate;
	FWidgetLifecycleEvent WidgetReleasingDelegate;
	
};


