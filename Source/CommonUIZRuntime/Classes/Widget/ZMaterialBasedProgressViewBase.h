// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Widget/ZUserWidgetBase.h"

#include "ZMaterialBasedProgressViewBase.generated.h"

class UImage;

UENUM(BlueprintType)
enum class EZMaterialBasedProgressViewAnimationType : uint8
{
	None,
	FixedSpeed,
	FixedTime,
};

UCLASS(Abstract, Blueprintable)
class COMMONUIZRUNTIME_API UZMaterialBasedProgressViewBase : public UZUserWidgetBase
{
	GENERATED_BODY()
	
	// TODO: FieldNotify
public:
	UFUNCTION(BlueprintPure)
	float GetValue() const;
	
	UFUNCTION(BlueprintCallable)
	void SetValue(float value);
	
	UFUNCTION(BlueprintPure)
	float GetMaxValue() const;
	
	UFUNCTION(BlueprintCallable)
	void SetMaxValue(float value);
	
	UFUNCTION(BlueprintPure)
	float GetPercentage() const;
	
	UFUNCTION(BlueprintCallable)
	void SetValueAndMaxValue(float value, float maxValue);
	
	UFUNCTION(BlueprintPure)
	FLinearColor GetFillColor() const;
	
	UFUNCTION(BlueprintCallable)
	void SetFillColor(const FLinearColor& value);
	
	UFUNCTION(BlueprintPure)
	FLinearColor GetDelayedFillColor() const;
	
	UFUNCTION(BlueprintCallable)
	void SetDelayedFillColor(const FLinearColor& value);
	
	UFUNCTION(BlueprintPure)
	FLinearColor GetBackgroundColor() const;
	
	UFUNCTION(BlueprintCallable)
	void SetBackgroundColor(const FLinearColor& value);
	
	UFUNCTION(BlueprintPure)
	FLinearColor GetBorderColor() const;
	
	UFUNCTION(BlueprintCallable)
	void SetBorderColor(const FLinearColor& value);
	
#if WITH_EDITOR
	
public:
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& compileLog) const override;
	
#endif
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float deltaTime) override;
	
private:
	UFUNCTION()
	bool CanEditAnimationType() const;
	
	UFUNCTION()
	bool CanEditAnimationControl() const;
	
private:
	float GetDelayedPercentage() const;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> UI_Image;
	
private:
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	TObjectPtr<UMaterialInterface> ProgressMaterial;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	FName PercentageParameter = "Percentage";
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	FName DelayedPercentageParameter = "DelayedPercentage";
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	FName FillColorParameter = "FillColor";
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	FName DelayedFillColorParameter = "DelayedFillColor";
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	FName BackgroundColorParameter = "BackgroundColor";
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Material")
	FName BorderColorParameter = "BorderColor";
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Animation", meta = (EditCondition = "CanEditAnimationType()", EditConditionHides))
	EZMaterialBasedProgressViewAnimationType AnimationType;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView|Animation", meta = (EditCondition = "CanEditAnimationControl()", EditConditionHides))
	float AnimationControl;
	
private:
	UPROPERTY(EditAnywhere, Category = "ProgressView", meta = (ClampMin = "0", UIMin = "0"))
	float Value = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView", meta = (ClampMin = "0", UIMin = "0"))
	float MaxValue = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView")
	FLinearColor FillColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView")
	FLinearColor DelayedFillColor = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView")
	FLinearColor BackgroundColor = FLinearColor::Black;
	
	UPROPERTY(EditAnywhere, Category = "ProgressView")
	FLinearColor BorderColor = FLinearColor::Gray;
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
	
	UPROPERTY(Transient)
	float DelayedValue;
	
};


