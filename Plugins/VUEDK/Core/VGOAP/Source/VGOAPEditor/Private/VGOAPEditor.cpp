// Copyright VUEDK, Inc. All Rights Reserved.

#include "VGOAPEditor.h"
#include "AssetToolsModule.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FVGOAPEditorModule"

uint32 FVGOAPEditorModule::GOAPCategory = 0;

void FVGOAPEditorModule::StartupModule()
{
	StyleSet = MakeShared<FSlateStyleSet>("VGOAPEditorStyle");
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin("VGOAP")->GetBaseDir() / TEXT("Resources"));

	StyleSet->Set(
		"BlackboardIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Blackboard128.png")), FVector2D(128.0f, 128.0f))
	);

	StyleSet->Set(
		"BehaviourIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Behaviour128.png")), FVector2D(128.0f, 128.0f))
	);

	// FAssetEditorToolkit::RegisterTabSpawners();
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	BehaviourAssetTypeAction = MakeShared<FAssetTypeActions_Behaviour>();
	BlackboardAssetTypeAction = MakeShared<FAssetTypeActions_Blackboard>();
	SensorAssetTypeAction = MakeShared<FAssetTypeActions_Sensor>();
	BeliefAssetTypeAction = MakeShared<FAssetTypeActions_Belief>();
	ActionAssetTypeAction = MakeShared<FAssetTypeActions_Action>();
	GoalAssetTypeAction = MakeShared<FAssetTypeActions_Goal>();

	AssetTools.RegisterAssetTypeActions(BehaviourAssetTypeAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(BlackboardAssetTypeAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(SensorAssetTypeAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(BeliefAssetTypeAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(ActionAssetTypeAction.ToSharedRef());
	AssetTools.RegisterAssetTypeActions(GoalAssetTypeAction.ToSharedRef());

	GOAPCategory = AssetTools.RegisterAdvancedAssetCategory(
		FName("VGOAP"),
		LOCTEXT("VGOAP", "VGOAP")
	);
}

void FVGOAPEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
		return;

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	AssetTools.UnregisterAssetTypeActions(BehaviourAssetTypeAction.ToSharedRef());
	AssetTools.UnregisterAssetTypeActions(BlackboardAssetTypeAction.ToSharedRef());
	AssetTools.UnregisterAssetTypeActions(SensorAssetTypeAction.ToSharedRef());
	AssetTools.UnregisterAssetTypeActions(BeliefAssetTypeAction.ToSharedRef());
	AssetTools.UnregisterAssetTypeActions(ActionAssetTypeAction.ToSharedRef());
	AssetTools.UnregisterAssetTypeActions(GoalAssetTypeAction.ToSharedRef());
}

TArray<FText>& FVGOAPEditorModule::GetGOAPSubMenus()
{
	static TArray<FText> SubMenus;
	if (SubMenus.Num() == 0)
		SubMenus.Add(LOCTEXT("GOAPSubMenu_Blueprints", "Blueprints"));
		
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVGOAPEditorModule, VGOAPEditor)
