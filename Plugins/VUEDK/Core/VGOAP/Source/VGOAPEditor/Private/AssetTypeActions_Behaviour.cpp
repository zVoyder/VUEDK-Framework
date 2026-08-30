// Copyright VUEDK, Inc. All Rights Reserved.

#include "AssetTypeActions_Behaviour.h"
#include "Data/GOAPAgentBehaviourData.h"
#include "Styling/SlateStyleRegistry.h"

UClass* FAssetTypeActions_Behaviour::GetSupportedClass() const
{
	return UGOAPAgentBehaviourData::StaticClass();
}

FColor FAssetTypeActions_Behaviour::GetTypeColor() const
{
	return FColor(149, 70, 255);
}

FText FAssetTypeActions_Behaviour::GetName() const
{
	return FText::FromString("GOAP Behaviour");
}

const FSlateBrush* FAssetTypeActions_Behaviour::GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const
{
	return FSlateStyleRegistry::FindSlateStyle("VGOAPEditorStyle")->GetBrush("BehaviourIcon");
}
