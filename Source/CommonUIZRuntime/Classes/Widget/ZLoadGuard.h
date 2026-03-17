// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonLoadGuard.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZLoadGuard.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZLoadGuard : public UCommonLoadGuard
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


