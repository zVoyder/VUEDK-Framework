using UnrealBuildTool;

public class EventsDispatcherSystemEditor : ModuleRules
{
	public EventsDispatcherSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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
			"EventsDispatcherSystem",
			"UnrealEd",
			"Kismet",
			"KismetCompiler",
			"BlueprintGraph",
			"GameplayTags"
		});
	}
}
