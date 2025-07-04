// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AISystem : ModuleRules
{
	public AISystem(ReadOnlyTargetRules Target) : base(Target)
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
			"Core"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"AIModule",
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
	}
}
