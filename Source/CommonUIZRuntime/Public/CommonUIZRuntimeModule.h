// Copyright Zero Games. All Rights Reserved.

#pragma once

class ICommonUIZRuntimeModule : public IModuleInterface
{
public:
	static FORCEINLINE ICommonUIZRuntimeModule& Get()
	{
		static ICommonUIZRuntimeModule& GSingleton = FModuleManager::LoadModuleChecked<ICommonUIZRuntimeModule>("CommonUIZRuntime");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("CommonUIZRuntime");
	}
};


