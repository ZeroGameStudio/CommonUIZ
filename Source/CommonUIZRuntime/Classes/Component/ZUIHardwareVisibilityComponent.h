// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonHardwareVisibilityBorder.h"
#include "GameplayTagContainer.h"
#include "Component/ZUIComponentBase.h"

#include "ZUIHardwareVisibilityComponent.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZUIHardwareVisibilityComponent : public UZUIComponentBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnConstruct() override;
	virtual void OnDestruct() override;
	virtual TSharedRef<SWidget> RebuildWidgetWithContent(TSharedRef<SWidget> content) override;
	
#if WITH_EDITOR
	
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
	
#endif
	
private:
	void UpdateVisibility(UCommonUIVisibilitySubsystem* subsystem = nullptr);
	
private:
	UPROPERTY(EditAnywhere, Category = "Visibility", meta=(Categories="Input,Platform.Trait"))
	FGameplayTagQuery VisibilityQuery;

	UPROPERTY(EditAnywhere, Category = "Visibility")
	ESlateVisibility VisibleType = ESlateVisibility::SelfHitTestInvisible;
	
	UPROPERTY(EditAnywhere, Category = "Visibility")
	ESlateVisibility HiddenType = ESlateVisibility::Collapsed;
	
private:
	TWeakPtr<SBox> MyWrapper;
	
};


