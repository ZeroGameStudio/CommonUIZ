// Copyright Zero Games. All Rights Reserved.

#include "Widget/ZButtonBase.h"

#include "Blueprint/WidgetTree.h"

UZButtonBase::UZButtonBase()
{
	SetVisibilityInternal(ESlateVisibility::Visible);
}

UCommonButtonInternalBase* UZButtonBase::ConstructInternalButton()
{
	auto button = WidgetTree->ConstructWidget<UZButtonInternalBase>(UZButtonInternalBase::StaticClass(), FName(TEXT("InternalRootButtonBase")));
	RootButton = button;
	
	return button;
}


