// Copyright Zero Games. All Rights Reserved.

#include "CommonUIZRuntimeModule.h"

class FCommonUIZRuntimeModule : public ICommonUIZRuntimeModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FCommonUIZRuntimeModule, CommonUIZRuntime)

void FCommonUIZRuntimeModule::StartupModule()
{
}

void FCommonUIZRuntimeModule::ShutdownModule()
{
}


