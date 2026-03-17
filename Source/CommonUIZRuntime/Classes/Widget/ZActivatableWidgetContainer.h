// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonUIZRuntimeTexts.h"
#include "Components/Widget.h"

#include "ZActivatableWidgetContainer.generated.h"

class UCommonActivatableWidget;
class UZUserWidgetCacheBaseInterface;

UCLASS(ClassGroup = "UI", Category = "Zero UI", DisplayName = "Activatable Widget Container", PrioritizeCategories = "Content")
class COMMONUIZRUNTIME_API UZActivatableWidgetContainer : public UWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetReleasingMCDelegate, UCommonActivatableWidget*, widget);

public:
	UZActivatableWidgetContainer();

public:
	UFUNCTION(BlueprintCallable)
	UCommonActivatableWidget* PushWidget(FName id, TSubclassOf<UCommonActivatableWidget> widgetClass, bool& newlyCreated);

	UFUNCTION(BlueprintCallable)
	void PopWidget(int32 times = 1);

	UFUNCTION(BlueprintCallable)
	void RemoveWidget(FName id);

	UFUNCTION(BlueprintCallable)
	void RemoveWidgetInstance(UCommonActivatableWidget* widget);

	UFUNCTION(BlueprintCallable)
	void ClearWidgets();

	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintPure)
	UCommonActivatableWidget* GetActiveWidget() const;
	
	UFUNCTION(BlueprintPure)
	UCommonActivatableWidget* GetActiveWidgetWithId(FName& id) const;

	UFUNCTION(BlueprintPure)
	UCommonActivatableWidget* GetWidget(FName id) const;

	UFUNCTION(BlueprintPure)
	int32 GetNumWidgets() const;
	
public:
	UPROPERTY(EditAnywhere, Category = "Event", meta = (IsBindableEvent))
	FWidgetReleasingMCDelegate OnWidgetReleasing;
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
	
private:
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	int32 GetDesiredActiveWidgetIndex() const;
	void InternalAddWidget(FName id, UCommonActivatableWidget* widget);
	void InternalRemoveWidget(FName id);
	void InternalAddWidgetToList(FName id, UCommonActivatableWidget* widget);
	void InternalRemoveWidgetFromList(FName id);
	void InternalActivateWidget(UCommonActivatableWidget* widget);
	void InternalDeactivateWidget(UCommonActivatableWidget* widget);
	void RefreshBackgroundVisibility();
	UZUserWidgetCacheBaseInterface& EnsureCache();
	void HandleWidgetDeactivated(UCommonActivatableWidget* widget);

private:
	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<UZUserWidgetCacheBaseInterface> CacheClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BackgroundClass;

private:
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<UZUserWidgetCacheBaseInterface> Cache;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<UUserWidget> Background;

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UCommonActivatableWidget>> Widgets;
	
	UPROPERTY(Transient)
	TArray<FName> SortedWidgets;

private:
	TSharedPtr<SOverlay> MyOverlay;
	TSharedPtr<SOverlay> MyBackgroundOverlay;
	
};


