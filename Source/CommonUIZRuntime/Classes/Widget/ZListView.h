// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonListView.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZListView.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZListView : public UCommonListView
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


