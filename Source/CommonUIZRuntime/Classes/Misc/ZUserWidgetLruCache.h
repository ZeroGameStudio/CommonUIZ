// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZUserWidgetCacheBaseInterface.h"

#include "ZUserWidgetLruCache.generated.h"

USTRUCT()
struct FZUserWidgetLruCacheRec
{
	GENERATED_BODY()

	friend bool operator<(const FZUserWidgetLruCacheRec& lhs, const FZUserWidgetLruCacheRec& rhs) { return lhs.Timestamp < rhs.Timestamp; }

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget;

	UPROPERTY()
	double Timestamp = 0;
};

UCLASS()
class COMMONUIZRUNTIME_API UZUserWidgetLruCache : public UZUserWidgetCacheBaseInterface
{
	GENERATED_BODY()

protected:
	virtual UUserWidget* GetActiveWidget(FName id) const override;
	virtual UUserWidget* GetInactiveWidget(FName id) const override;

	virtual void ActivateNewWidget(FName id, UUserWidget* widget) override;
	virtual UUserWidget* ActivateExistingWidget(FName id) override;
	virtual UUserWidget* DeactivateExistingWidget(FName id, bool& released) override;

	virtual void ReleaseAllWidgets(TFunctionRef<void(UUserWidget*, bool)> callback) override;
	virtual void ForeachActiveWidget(TFunctionRef<void(UUserWidget*)> action) const override;

	virtual void Resetting() override;

private:
	void Trim();

private:
	UPROPERTY(EditDefaultsOnly, Category = "LRU Cache")
	int32 MaxInactiveCount = 8;

	UPROPERTY(EditDefaultsOnly, Category = "LRU Cache")
	double MaxAliveSeconds = 300;

private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UUserWidget>> ActiveWidgetMap;

	UPROPERTY()
	TMap<FName, FZUserWidgetLruCacheRec> InactiveWidgetMap;

private:
	FTSTicker::FDelegateHandle TimerDelegate;
	
};


