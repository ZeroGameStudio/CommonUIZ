// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "CommonUIZRuntimeTexts.h"
#include "ZSharpFieldNotifyObject.h"

#include "ZActivatableUserWidgetBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONUIZRUNTIME_API UZActivatableUserWidgetBase : public UCommonActivatableWidget, public IZSharpFieldNotifyObject
{
	GENERATED_BODY()

public:
	virtual const UE::FieldNotification::IClassDescriptor& GetFieldNotificationDescriptor() const override;

public:
	UZActivatableUserWidgetBase();
	
public:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	
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
	UFUNCTION()
	void Uninitialize();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	FText ScriptGetPaletteCategory() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUninitializing();

private:
	bool bInitialized;
	bool bUninitialized;
	FName CachedName;
	
};


