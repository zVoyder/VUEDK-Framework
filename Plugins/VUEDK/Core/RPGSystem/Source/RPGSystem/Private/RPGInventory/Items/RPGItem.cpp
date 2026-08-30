// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Items/RPGItem.h"
#include "RPGInventory/Data/RPGItemData.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

URPGItem::URPGItem()
{
	RarityLevel = nullptr;
	VisualDetails = FRPGItemVisualDetails();
}

void URPGItem::Init(const TObjectPtr<UWorld> InWorld, UItemDataBase* Data)
{
	Super::Init(InWorld, Data);

	const URPGRegistrySubsystem* RegistrySubsystem = GetDefault<URPGRegistrySubsystem>();
	const URPGItemData* RPGItemData = GetRPGItemData();
	if (RPGItemData->bUseRarity)
	{
		if (RPGItemData->bUsePredefinedRarity && IsValid(RPGItemData->PredefinedRarityLevelData))
		{
			RarityLevel = RPGItemData->PredefinedRarityLevelData;
			return;
		}
	
		RarityLevel = RegistrySubsystem->GetDefaultRarityLevel();
	}
}

void URPGItem::NativeOnPostGeneration()
{
	SetItemMeshToLoad();
	OnPostGeneration();
}

FText URPGItem::GetItemFullName() const
{
	const FText ItemName = VisualDetails.Name.IsEmptyOrWhitespace() ? Super::GetItemFullName() : VisualDetails.Name;

	if (RarityLevel == nullptr)
		return ItemName;

	return FText::Format(FText::FromString("{0} {1}"), RarityLevel->RarityName, ItemName);
}

FSlateBrush URPGItem::GetItemIcon() const
{
	return VisualDetails.Icon.GetResourceObject() != nullptr ? VisualDetails.Icon : Super::GetItemIcon();
}

FText URPGItem::GetItemDescription() const
{
	return VisualDetails.Description.IsEmpty() ? Super::GetItemDescription() : VisualDetails.Description;
}

URPGItemData* URPGItem::GetRPGItemData() const
{
	return Cast<URPGItemData>(ItemData);
}

bool URPGItem::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	if (!Super::CanStackItem_Implementation(OtherItem))
		return false;

	const URPGItem* OtherRPGItem = Cast<URPGItem>(OtherItem);

	if (OtherRPGItem == nullptr)
		return false;

	const bool bHasSameRarity = OtherRPGItem->RarityLevel == RarityLevel;
	const bool bHasSameVisualDetails = OtherRPGItem->VisualDetails == VisualDetails;

	return bHasSameRarity && bHasSameVisualDetails;
}

void URPGItem::SetItemMeshToLoad()
{
	if (VisualDetails.ItemMesh == nullptr)
	{
		Super::SetItemMeshToLoad();
		return;
	}

	ItemMesh = VisualDetails.ItemMesh;
}
