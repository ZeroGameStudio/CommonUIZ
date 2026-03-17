// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonRichTextBlock.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZRichTextBlock.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZRichTextBlock : public UCommonRichTextBlock
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


