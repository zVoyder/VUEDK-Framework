// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Items/RPGItem.h"
#include "Utility/ISInventoriesUtility.h"
#include "RPGInventory/Data/RPGItemData.h"
#include "RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"
#include "RPGItemsGeneration/Data/Rarities/RPGItemsRaritiesData.h"

URPGItem::URPGItem()
{
	RarityLevel = nullptr;
	VisualDetails = FRPGItemVisualDetails();
}

void URPGItem::Init(UObject* WorldContextObject, UItemDataBase* Data)
{
	Super::Init(WorldContextObject, Data);

	const URPGInventoriesManager* InventoriesManager = URPGInventoriesUtility::GetRPGInventoriesManager();
	if (!IsValid(InventoriesManager))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("URPGItem::Init: RPGInventoriesManager not found. Item %s cannot be initialized."), *GetItemFullName().ToString());
		return;
	}

	const URPGItemsRaritiesData* RaritiesData = InventoriesManager->ItemsRarities;

	if (!IsValid(RaritiesData))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("URPGItem::Init: Rarities data is not valid for item %s."), *GetItemFullName().ToString());
		return;
	}

	if (RaritiesData->RarityLevels.IsEmpty())
		return;

	const URPGItemData* RPGItemData = GetRPGItemData();
	if (RPGItemData->bUseRarity)
	{
		if (RPGItemData->bUsePredefinedRarity && IsValid(RPGItemData->PredefinedRarityLevelData))
		{
			RarityLevel = RPGItemData->PredefinedRarityLevelData;
			return;
		}

		RarityLevel = RaritiesData->DefaultRarityLevel; // Default rarity level
	}
}

void URPGItem::NativeOnPostGeneration()
{
	SetItemMeshToLoad();
	OnPostGeneration();
}

FRPGItemSaveData URPGItem::CreateRPGItemSaveData() const
{
	FRPGItemSaveData RPGItemSaveData;
	RPGItemSaveData.TetrisItemSaveData = CreateTetrisSaveData();
	RPGItemSaveData.VisualDetails = VisualDetails;

	if (GetRPGItemData()->bUseRarity)
	{
		if (IsValid(RarityLevel))
			RPGItemSaveData.RarityID = RarityLevel->RarityID;
		else
			UE_LOG(LogInventorySystem, Error, TEXT("URPGItem::CreateRPGItemSaveData: Trying to save an innvalid Rarity level for item %s."), *GetItemFullName().ToString());
	}

	return RPGItemSaveData;
}

void URPGItem::LoadRPGItemSaveData(URPGInventory* LoadingInventory, const FRPGItemSaveData& RPGItemSaveData)
{
	VisualDetails = RPGItemSaveData.VisualDetails;
	RarityLevel = URPGInventoriesUtility::GetItemRarityByID(RPGItemSaveData.RarityID);
	LoadTetrisSaveData(LoadingInventory, RPGItemSaveData.TetrisItemSaveData); // Call this after setting VisualDetails so that the item's icon is set correctly
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
