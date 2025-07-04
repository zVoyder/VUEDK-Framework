// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomSettingsSystem : ModuleRules
{
	public CustomSettingsSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"Settings",
			"EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
	}
}