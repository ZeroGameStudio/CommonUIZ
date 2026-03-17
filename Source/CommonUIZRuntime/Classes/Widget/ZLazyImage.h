// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonLazyImage.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZLazyImage.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZLazyImage : public UCommonLazyImage
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


