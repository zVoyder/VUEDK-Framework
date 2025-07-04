// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WeaponSystem : ModuleRules
{
	public WeaponSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"ObjectPool",
			"GameplayTags",
			"CrosshairSystem",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
	}
}
