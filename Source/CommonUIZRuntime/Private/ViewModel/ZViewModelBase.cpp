// Copyright Zero Games. All Rights Reserved.

#include "ViewModel/ZViewModelBase.h"

const UE::FieldNotification::IClassDescriptor& UZViewModelBase::GetFieldNotificationDescriptor() const
{
	static TFieldNotificationDescriptor<Super::FFieldNotificationClassDescriptor> Instance;
	return Instance;
}


