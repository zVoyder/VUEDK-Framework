// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ObjectPool : ModuleRules
{
	public ObjectPool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore"
		});
		
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PublicDependencyModuleNames.Add("Settings");
		}

#if WITH_EDITOR
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"LevelEditor"
		});
#endif
	}
}
