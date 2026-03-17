// Copyright Zero Games. All Rights Reserved.

#include "Component/ZUIHardwareVisibilityComponent.h"

#include "CommonUISettings.h"
#include "CommonUIVisibilitySubsystem.h"
#include "CommonUIZRuntimeLogChannels.h"
#include "ICommonUIModule.h"

void UZUIHardwareVisibilityComponent::OnConstruct()
{
	Super::OnConstruct();
	
	if (UWidget* owner = GetOwner().Get())
	{
		if (UCommonUIVisibilitySubsystem* subsystem = UCommonUIVisibilitySubsystem::Get(owner->GetOwningLocalPlayer()))
		{
			subsystem->OnVisibilityTagsChanged.AddWeakLambda(this, [this](UCommonUIVisibilitySubsystem* subsystem){ UpdateVisibility(subsystem); });
		}
	}
}

void UZUIHardwareVisibilityComponent::OnDestruct()
{
	if (UWidget* owner = GetOwner().Get())
	{
		if (UCommonUIVisibilitySubsystem* subsystem = UCommonUIVisibilitySubsystem::Get(owner->GetOwningLocalPlayer()))
		{
			subsystem->OnVisibilityTagsChanged.RemoveAll(this);
		}
	}
	
	Super::OnDestruct();
}

TSharedRef<SWidget> UZUIHardwareVisibilityComponent::RebuildWidgetWithContent(TSharedRef<SWidget> content)
{
	TSharedRef<SBox> wrapper = SNew(SBox)[content];
	MyWrapper = wrapper;
	UpdateVisibility();
	
	return wrapper;
}

#if WITH_EDITOR

void UZUIHardwareVisibilityComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);
	
	UpdateVisibility();
}

#endif

void UZUIHardwareVisibilityComponent::UpdateVisibility(UCommonUIVisibilitySubsystem* subsystem)
{
	TSharedPtr<SBox> wrapper = MyWrapper.Pin();
	if (!wrapper)
	{
		return;
	}
	
	bool visible = true;
	if (!VisibilityQuery.IsEmpty())
	{
		if (!subsystem)
		{
			if (UWidget* owner = GetOwner().Get())
			{
				subsystem = UCommonUIVisibilitySubsystem::Get(owner->GetOwningLocalPlayer());
			}
		}
		
		if (subsystem)
		{
			visible = VisibilityQuery.Matches(subsystem->GetVisibilityTags());
		}
		else
		{
			UE_LOG(LogCommonUIZRuntime, Verbose, TEXT("[%s] -> ULocalPlayer not available, using PlatformTraits instead"), *GetName());
			
			const FGameplayTagContainer& traits = ICommonUIModule::GetSettings().GetPlatformTraits();
			visible = VisibilityQuery.Matches(traits);
		}
	}

	wrapper->SetVisibility(UWidget::ConvertSerializedVisibilityToRuntime(visible ? VisibleType : HiddenType));
}


