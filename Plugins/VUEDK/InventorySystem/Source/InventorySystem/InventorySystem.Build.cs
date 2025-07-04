// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventorySystem : ModuleRules
{
	public InventorySystem(ReadOnlyTargetRules Target) : base(Target)
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
			"SaveSystem",
			"UMG",
			"BetterDragSystem",
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
