// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonTreeView.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZTreeView.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZTreeView : public UCommonTreeView
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
};


