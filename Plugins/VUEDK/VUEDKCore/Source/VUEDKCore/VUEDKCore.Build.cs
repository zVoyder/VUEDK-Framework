// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VUEDKCore : ModuleRules
{
	public VUEDKCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});

		// VUEDK Modules
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"SaveSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore"
		});
	}
}
