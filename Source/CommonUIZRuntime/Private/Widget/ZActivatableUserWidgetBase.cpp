// Copyright Zero Games. All Rights Reserved.

#include "Widget/ZActivatableUserWidgetBase.h"

#include "CommonUIZRuntimeLogChannels.h"

const UE::FieldNotification::IClassDescriptor& UZActivatableUserWidgetBase::GetFieldNotificationDescriptor() const
{
	static TFieldNotificationDescriptor<Super::FFieldNotificationClassDescriptor> Instance;
	return Instance;
}

UZActivatableUserWidgetBase::UZActivatableUserWidgetBase()
	: bInitialized(false)
	, bUninitialized(false)
	, CachedName(GetFName())
{
#if WITH_EDITORONLY_DATA
	PaletteCategory = CommonUIZRuntime::GText_WidgetPaletteCategory;
#endif
}

TOptional<FUIInputConfig> UZActivatableUserWidgetBase::GetDesiredInputConfig() const
{
	return BP_GetDesiredInputConfig();
}

void UZActivatableUserWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Fix this flag for Z# class.
	bHasScriptImplementedTick = FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, Tick))->GetOuterUClass() != StaticClass();
	
	bInitialized = true;
}

void UZActivatableUserWidgetBase::BeginDestroy()
{
	if (bInitialized && !bUninitialized)
	{
		ensureAlways(false);
		UE_LOG(LogCommonUIZRuntime, Error, TEXT("Widget [%s] needs to be explicitly uninitialized!"), *CachedName.ToString());
	}
	
	Super::BeginDestroy();
}

void UZActivatableUserWidgetBase::Uninitialize()
{
	if (!bInitialized)
	{
		UE_LOG(LogCommonUIZRuntime, Warning, TEXT("Widget [%s] has already been uninitialized."), *GetNameSafe(this));
		return;
	}

	if (bUninitialized)
	{
		UE_LOG(LogCommonUIZRuntime, Warning, TEXT("Widget [%s] has already been uninitialized."), *GetNameSafe(this));
		return;
	}

	RemoveFromParent();

	OnUninitializing();
	
	bUninitialized = true;
}


