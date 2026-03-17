// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonTileView.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZTileView.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZTileView : public UCommonTileView
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


