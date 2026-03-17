// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonUIZRuntimeTexts.h"
#include "Components/Image.h"

#include "ZImage.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZImage : public UImage
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


