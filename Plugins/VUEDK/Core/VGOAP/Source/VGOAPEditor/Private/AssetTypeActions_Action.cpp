// Copyright VUEDK, Inc. All Rights Reserved.

#include "AssetTypeActions_Action.h"
#include "VGOAPEditor.h"
#include "Agent/AgentAction.h"

UClass* FAssetTypeActions_Action::GetSupportedClass() const
{
	return UAgentAction::StaticClass();
}

FText FAssetTypeActions_Action::GetName() const
{
	return FText::FromString("Action");
}

const TArray<FText>& FAssetTypeActions_Action::GetSubMenus() const
{
	return FVGOAPEditorModule::GetGOAPSubMenus();
}
