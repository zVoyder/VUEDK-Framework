// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LoadingScreenSystem : ModuleRules
{
	public LoadingScreenSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
		});

		PrivateIncludePaths.AddRange(new string[]
		{
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"MoviePlayer",
			"UMG",
			"EngineSettings"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
		
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PublicDependencyModuleNames.Add("Settings");
		}
	}
}
