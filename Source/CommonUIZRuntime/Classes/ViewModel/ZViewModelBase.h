// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "MVVMViewModelBase.h"
#include "ZSharpFieldNotifyObject.h"

#include "ZViewModelBase.generated.h"

UCLASS(Abstract)
class COMMONUIZRUNTIME_API UZViewModelBase : public UMVVMViewModelBase, public IZSharpFieldNotifyObject
{
	GENERATED_BODY()

public:
	virtual const UE::FieldNotification::IClassDescriptor& GetFieldNotificationDescriptor() const override;
	
};
