// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BetterDragSystem : ModuleRules
{
	public BetterDragSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"InputCore"
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
