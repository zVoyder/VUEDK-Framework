// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGSystem : ModuleRules
{
	public RPGSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"InventorySystem"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"DeveloperSettings"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
	}
}
