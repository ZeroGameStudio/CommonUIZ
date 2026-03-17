// Copyright Zero Games. All Rights Reserved.

#include "Widget/ZMaterialBasedProgressViewBase.h"

#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Components/Image.h"
#include "Editor/WidgetCompilerLog.h"

namespace CommonUIZRuntime::ZMaterialBasedProgressViewBase_Private
{
	static float CalcPercentage(float value, float maxValue)
	{
		if (maxValue == 0)
		{
			return value > 0 ? 1 : 0;
		}
	
		return FMath::Clamp(value / maxValue, 0, 1);
	}
}

float UZMaterialBasedProgressViewBase::GetValue() const
{
	return Value;
}

void UZMaterialBasedProgressViewBase::SetValue(float value)
{
	if (Value == value)
	{
		return;
	}
	
	Value = FMath::Max(value, 0); // Don't clamp to MaxValue because they may change independently.
	if (AnimationType == EZMaterialBasedProgressViewAnimationType::None || Value > DelayedValue)
	{
		DelayedValue = Value;
	}
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(PercentageParameter, GetPercentage());
		DynamicMaterial->SetScalarParameterValue(DelayedPercentageParameter, GetDelayedPercentage());
	}
}

float UZMaterialBasedProgressViewBase::GetMaxValue() const
{
	return MaxValue;
}

void UZMaterialBasedProgressViewBase::SetMaxValue(float value)
{
	if (MaxValue == value)
	{
		return;
	}
	
	MaxValue = FMath::Max(value, 0);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(PercentageParameter, GetPercentage());
		DynamicMaterial->SetScalarParameterValue(DelayedPercentageParameter, GetDelayedPercentage());
	}
}

float UZMaterialBasedProgressViewBase::GetPercentage() const
{
	return CommonUIZRuntime::ZMaterialBasedProgressViewBase_Private::CalcPercentage(Value, MaxValue);
}

void UZMaterialBasedProgressViewBase::SetValueAndMaxValue(float value, float maxValue)
{
	if (Value == value && MaxValue == maxValue)
	{
		return;
	}
	
	MaxValue = FMath::Max(maxValue, 0);
	Value = FMath::Clamp(value, 0, MaxValue);
	if (AnimationType == EZMaterialBasedProgressViewAnimationType::None || Value > DelayedValue)
	{
		DelayedValue = Value;
	}
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(PercentageParameter, GetPercentage());
		DynamicMaterial->SetScalarParameterValue(DelayedPercentageParameter, GetDelayedPercentage());
	}
}

FLinearColor UZMaterialBasedProgressViewBase::GetFillColor() const
{
	return FillColor;
}

void UZMaterialBasedProgressViewBase::SetFillColor(const FLinearColor& value)
{
	if (FillColor == value)
	{
		return;
	}
	
	FillColor = value;
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FillColorParameter, FillColor);
	}
}

FLinearColor UZMaterialBasedProgressViewBase::GetDelayedFillColor() const
{
	return DelayedFillColor;
}

void UZMaterialBasedProgressViewBase::SetDelayedFillColor(const FLinearColor& value)
{
	if (DelayedFillColor == value)
	{
		return;
	}
	
	DelayedFillColor = value;
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(DelayedFillColorParameter, DelayedFillColor);
	}
}

FLinearColor UZMaterialBasedProgressViewBase::GetBackgroundColor() const
{
	return BackgroundColor;
}

void UZMaterialBasedProgressViewBase::SetBackgroundColor(const FLinearColor& value)
{
	if (BackgroundColor == value)
	{
		return;
	}
	
	BackgroundColor = value;
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(BackgroundColorParameter, BackgroundColor);
	}
}

FLinearColor UZMaterialBasedProgressViewBase::GetBorderColor() const
{
	return BorderColor;
}

void UZMaterialBasedProgressViewBase::SetBorderColor(const FLinearColor& value)
{
	if (BorderColor == value)
	{
		return;
	}
	
	BorderColor = value;
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(BorderColorParameter, BorderColor);
	}
}

#if WITH_EDITOR

void UZMaterialBasedProgressViewBase::ValidateCompiledDefaults(IWidgetCompilerLog& compileLog) const
{
	Super::ValidateCompiledDefaults(compileLog);
	
	auto bpgc = Cast<const UWidgetBlueprintGeneratedClass>(GetClass());
	if (bpgc)
	{
		if (AnimationType != EZMaterialBasedProgressViewAnimationType::None && !bpgc->ClassRequiresNativeTick())
		{
			compileLog.Error(FText::Format(NSLOCTEXT("CommonUIZRuntime", "Error_MaterialBasedProgressView_RequiresNativeTick", "{0} has animation but without native tick enabled."), FText::FromString(GetName())));
		}
	}
}

#endif

void UZMaterialBasedProgressViewBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	DelayedValue = Value;
	
	if (UI_Image)
	{
		UI_Image->SetBrushResourceObject(ProgressMaterial);
		DynamicMaterial = UI_Image->GetDynamicMaterial();
		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue(PercentageParameter, GetPercentage());
			DynamicMaterial->SetScalarParameterValue(DelayedPercentageParameter, GetDelayedPercentage());
			DynamicMaterial->SetVectorParameterValue(FillColorParameter, FillColor);
			DynamicMaterial->SetVectorParameterValue(DelayedFillColorParameter, DelayedFillColor);
			DynamicMaterial->SetVectorParameterValue(BackgroundColorParameter, BackgroundColor);
			DynamicMaterial->SetVectorParameterValue(BorderColorParameter, BorderColor);
		}
	}
}

void UZMaterialBasedProgressViewBase::NativeTick(const FGeometry& geometry, float deltaTime)
{
	Super::NativeTick(geometry, deltaTime);
	
	if (DelayedValue == Value)
	{
		return;
	}
	
	bool changed = false;
	if (AnimationType != EZMaterialBasedProgressViewAnimationType::FixedSpeed)
	{
		DelayedValue = FMath::Clamp(DelayedValue + AnimationControl * deltaTime, DelayedValue, Value);
		changed = true;
	}
	else if (AnimationType == EZMaterialBasedProgressViewAnimationType::FixedTime)
	{
		// TODO
		DelayedValue = FMath::Clamp(DelayedValue + AnimationControl * deltaTime, DelayedValue, Value);
		changed = true;
	}
	
	if (changed)
	{
		DynamicMaterial->SetScalarParameterValue(DelayedPercentageParameter, GetDelayedPercentage());
	}
}

bool UZMaterialBasedProgressViewBase::CanEditAnimationType() const
{
	auto bpgc = Cast<const UWidgetBlueprintGeneratedClass>(GetClass());
	return bpgc && bpgc->ClassRequiresNativeTick();
}

bool UZMaterialBasedProgressViewBase::CanEditAnimationControl() const
{
	return CanEditAnimationType() && AnimationType != EZMaterialBasedProgressViewAnimationType::None;
}

float UZMaterialBasedProgressViewBase::GetDelayedPercentage() const
{
	return CommonUIZRuntime::ZMaterialBasedProgressViewBase_Private::CalcPercentage(DelayedValue, MaxValue);
}


