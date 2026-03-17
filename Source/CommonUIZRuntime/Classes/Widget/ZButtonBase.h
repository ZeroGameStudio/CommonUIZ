// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"
#include "CommonUIZRuntimeTexts.h"

#include "ZButtonBase.generated.h"

UCLASS(Experimental)
class COMMONUIZRUNTIME_API UZButtonInternalBase : public UCommonButtonInternalBase
{
	GENERATED_BODY()
};

UCLASS(Abstract)
class COMMONUIZRUNTIME_API UZButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UZButtonBase();
	
protected:
	UFUNCTION(BlueprintCallable)
	UButton* GetRootButton() const { return RootButton.Get(); }
	
protected:
	virtual UCommonButtonInternalBase* ConstructInternalButton() override;
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override { return CommonUIZRuntime::GText_WidgetPaletteCategory; }

#endif
	
private:
	TWeakObjectPtr<UButton> RootButton;
	
};


