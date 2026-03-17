// Copyright Zero Games. All Rights Reserved.

#include "Widget/ZUserWidgetBase.h"

#include "CommonUIZRuntimeLogChannels.h"

UZUserWidgetBase::UZUserWidgetBase()
{
#if WITH_EDITORONLY_DATA
	PaletteCategory = CommonUIZRuntime::GText_WidgetPaletteCategory;
#endif
}

const UE::FieldNotification::IClassDescriptor& UZUserWidgetBase::GetFieldNotificationDescriptor() const
{
	static TFieldNotificationDescriptor<Super::FFieldNotificationClassDescriptor> Instance;
	return Instance;
}

void UZUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (bNotifyClickOutside)
	{
		ClickOutsideDelegate = FSlateApplication::Get().GetPopupSupport().RegisterClickNotification(TakeWidget(), FOnClickedOutside::CreateUObject(this, &ThisClass::NativeHandleClickOutside));
	}
}

void UZUserWidgetBase::NativeDestruct()
{
	if (ClickOutsideDelegate.IsValid())
	{
		FSlateApplication::Get().GetPopupSupport().UnregisterClickNotification(ClickOutsideDelegate);
		ClickOutsideDelegate.Reset();
	}
	
	Super::NativeDestruct();
}

void UZUserWidgetBase::NativeHandleClickOutside()
{
	UE_LOG(LogCommonUIZRuntime, Log, TEXT("Click outside of [%s] detected."), *GetNameSafe(this));
	
	HandleClickOutside();
}


