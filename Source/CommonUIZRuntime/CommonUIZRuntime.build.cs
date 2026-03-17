// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class CommonUIZRuntime : ModuleRules
{
	public CommonUIZRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PrivateIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
				"CoreUObject",
				"InputCore",
				"SlateCore",
				"Slate",
				"Engine",
				"GameplayTags",
				"UMG",
				
				"ModelViewViewModel",
				"CommonInput",
				"CommonUI",
				
				"ZSharpCore",
				"ZSharpRuntime",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{

			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
	}
}


