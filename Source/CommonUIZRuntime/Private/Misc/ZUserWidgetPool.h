// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZUserWidgetPool.generated.h"

class UWidget;

UENUM(BlueprintType)
enum class EZUserWidgetPoolExceedPolicy : uint8
{
	Create,
	Preempt,
	ReturnNull,
};

UINTERFACE(Blueprintable)
class UZPoolableUserWidget : public UInterface
{
	GENERATED_BODY()
};

class COMMONUIZRUNTIME_API IZPoolableUserWidget : public IInterface
{
	GENERATED_BODY()
	
	friend class UZUserWidgetPool;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	bool PoolableUserWidget_AllowsPreempt() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void PoolableUserWidget_Initialize();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PoolableUserWidget_Uninitialize(bool preempt);
	
	UFUNCTION(BlueprintImplementableEvent)
	UZUserWidgetPool* GetPool() const;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPool(UZUserWidgetPool* pool);
	
};

UCLASS(Abstract)
class COMMONUIZRUNTIME_API UZUserWidgetPoolLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "widget"))
	static void Return(TScriptInterface<IZPoolableUserWidget> widget);
};

UCLASS(BlueprintType)
class COMMONUIZRUNTIME_API UZUserWidgetPool : public UObject
{
	GENERATED_BODY()
	
	friend class UZUserWidgetPoolLibrary;
	
public:
	UFUNCTION(BlueprintCallable, DisplayName = "Create User Widget Pool", meta = (DefaultToSelf = "owner", AdvancedDisplay = "2"))
	static UZUserWidgetPool* Create(
		UWidget* owner,
		UPARAM(meta = (MustImplement = "/Script/CommonUIZRuntime.ZPoolableUserWidget")) TSubclassOf<UUserWidget> pooledWidgetClass,
		int32 capacity = 64,
		int32 precacheCount = 16,
		EZUserWidgetPoolExceedPolicy exceedPolicy = EZUserWidgetPoolExceedPolicy::Create);
	
public:
	UZUserWidgetPool();
	
public:
	UFUNCTION(BlueprintCallable)
	UUserWidget* Rent();
	
private:
	void Initialize();
	
private:
	UUserWidget* Create();
	UUserWidget* Preempt();
	void Return(UUserWidget* widget);
	
private:
	UPROPERTY()
	TObjectPtr<UWidget> Owner;
	
	UPROPERTY()
	TSubclassOf<UUserWidget> PooledWidgetClass;
	
	UPROPERTY()
	int32 Capacity;
	
	UPROPERTY()
	int32 PrecacheCount;
	
	UPROPERTY()
	EZUserWidgetPoolExceedPolicy ExceedPolicy;
	
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> CachedWidgets;
	
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> RentedWidgets;
	
};
