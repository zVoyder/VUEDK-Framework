// Copyright VUEDK, Inc. All Rights Reserved.

#include "AssetTypeActions_Belief.h"
#include "VGOAPEditor.h"
#include "Agent/AgentBelief.h"

UClass* FAssetTypeActions_Belief::GetSupportedClass() const
{
	return UAgentBelief::StaticClass();
}

FText FAssetTypeActions_Belief::GetName() const
{
	return FText::FromString("Belief");
}

const TArray<FText>& FAssetTypeActions_Belief::GetSubMenus() const
{
	return FVGOAPEditorModule::GetGOAPSubMenus();
}
