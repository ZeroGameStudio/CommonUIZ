// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonLazyWidget.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZLazyWidget.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZLazyWidget : public UCommonLazyWidget
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


