// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonActivatableWidgetSwitcher.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZWidgetSwitcher.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZWidgetSwitcher : public UCommonActivatableWidgetSwitcher
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


