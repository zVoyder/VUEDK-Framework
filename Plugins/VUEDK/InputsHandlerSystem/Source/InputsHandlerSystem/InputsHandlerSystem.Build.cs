// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InputsHandlerSystem : ModuleRules
{
	public InputsHandlerSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UMG",
			"InputCore",
			"EnhancedInput",
			"Settings"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore"
		});
	}
}
