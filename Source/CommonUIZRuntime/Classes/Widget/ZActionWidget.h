// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonActionWidget.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZActionWidget.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
	
};


