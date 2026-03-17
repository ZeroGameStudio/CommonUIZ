// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonUIZRuntimeTexts.h"
#include "CommonUserWidget.h"
#include "ZSharpFieldNotifyObject.h"

#include "ZUserWidgetBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONUIZRUNTIME_API UZUserWidgetBase : public UCommonUserWidget, public IZSharpFieldNotifyObject
{
	GENERATED_BODY()
	
public:
	UZUserWidgetBase();

public:
	virtual const UE::FieldNotification::IClassDescriptor& GetFieldNotificationDescriptor() const override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
#if WITH_EDITOR
	
protected:
	virtual const FText GetPaletteCategory() override
	{
		FText text = ScriptGetPaletteCategory();
		if (!text.IsEmpty())
		{
			return text;
		}
		
		return PaletteCategory;
	}

#endif

protected:
	virtual void NativeHandleClickOutside();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	FText ScriptGetPaletteCategory() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleClickOutside();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bNotifyClickOutside;

private:
	FDelegateHandle ClickOutsideDelegate;
	
};


