// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/AssetTypeActions_GOAPBase.h"
#include "VGOAPEditor.h"

FText FAssetTypeActions_GOAPBase::GetName() const
{
	return FText::FromString("GOAP");
}

FColor FAssetTypeActions_GOAPBase::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FAssetTypeActions_GOAPBase::GetCategories()
{
	return FVGOAPEditorModule::GOAPCategory;
}
