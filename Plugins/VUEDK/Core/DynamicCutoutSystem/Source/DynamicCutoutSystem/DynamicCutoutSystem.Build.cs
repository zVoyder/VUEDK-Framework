// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DynamicCutoutSystem : ModuleRules
{
	public DynamicCutoutSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore"
		});
	}
}
