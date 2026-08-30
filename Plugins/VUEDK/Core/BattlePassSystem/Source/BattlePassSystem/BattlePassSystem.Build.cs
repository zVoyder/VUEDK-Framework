// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BattlePassSystem : ModuleRules
{
	public BattlePassSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"Json",
			"JsonUtilities",
			"OnlineSubsystem",
			"PlatformCryptoOpenSSL",
			"PlatformCryptoContext"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
		
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PublicDependencyModuleNames.Add("Settings");
		}
	}
}