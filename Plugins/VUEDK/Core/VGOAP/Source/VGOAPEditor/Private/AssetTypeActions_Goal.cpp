// Copyright VUEDK, Inc. All Rights Reserved.

#include "AssetTypeActions_Goal.h"
#include "VGOAPEditor.h"
#include "Agent/AgentGoal.h"

UClass* FAssetTypeActions_Goal::GetSupportedClass() const
{
	return UAgentGoal::StaticClass();
}

FText FAssetTypeActions_Goal::GetName() const
{
	return FText::FromString("Goal");
}

const TArray<FText>& FAssetTypeActions_Goal::GetSubMenus() const
{
	return FVGOAPEditorModule::GetGOAPSubMenus();
}
