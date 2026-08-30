// Copyright VUEDK, Inc. All Rights Reserved.

#include "AssetTypeActions_Blackboard.h"
#include "Data/GOAPBlackboardData.h"
#include "Styling/SlateStyleRegistry.h"

UClass* FAssetTypeActions_Blackboard::GetSupportedClass() const
{
	return UGOAPBlackboardData::StaticClass();
}

FColor FAssetTypeActions_Blackboard::GetTypeColor() const
{
	return FColor(201, 29, 85);
}

FText FAssetTypeActions_Blackboard::GetName() const
{
	return FText::FromString("GOAP Blackboard");
}

const FSlateBrush* FAssetTypeActions_Blackboard::GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const
{
	return FSlateStyleRegistry::FindSlateStyle("VGOAPEditorStyle")->GetBrush("BlackboardIcon");
}
