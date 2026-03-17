// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonTextBlock.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZTextBlock.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZTextBlock : public UCommonTextBlock
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


