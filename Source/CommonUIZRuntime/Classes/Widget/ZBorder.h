// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonBorder.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZBorder.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZBorder : public UCommonBorder
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


