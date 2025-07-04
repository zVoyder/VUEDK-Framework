// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BetterUI : ModuleRules
{
	public BetterUI(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"GameplayTags",
			"Settings"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
	}
}
