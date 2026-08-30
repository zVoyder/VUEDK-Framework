// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VGOAPEditor : ModuleRules
{
	public VGOAPEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"VGOAP"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Projects",
			"UnrealEd",
			"AssetTools",
			"Slate",
			"SlateCore",
			"WorkspaceMenuStructure"
		});
	}
}
