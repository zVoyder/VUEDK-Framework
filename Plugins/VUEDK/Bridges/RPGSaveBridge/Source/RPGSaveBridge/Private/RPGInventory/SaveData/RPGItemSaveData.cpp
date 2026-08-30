// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/SaveData/RPGItemSaveData.h"
#include "RPGSaveBridge.h"
#include "RPGInventory/Data/RPGItemData.h"
#include "RPGInventory/Items/RPGItem.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

bool URPGItemSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const URPGItem* RPGItem = Cast<URPGItem>(ObjectToSave);
	if (!IsValid(RPGItem))
		return false;

	const bool bUseRarity = IsValid(RPGItem->GetRPGItemData()) ? RPGItem->GetRPGItemData()->bUseRarity : false;
	SavedVisualDetails = RPGItem->VisualDetails;

	if (bUseRarity)
	{
		if (IsValid(RPGItem->RarityLevel))
			SavedRarityID = RPGItem->RarityLevel->RarityId;
		else
			UE_LOG(LogRPGSaveBridge, Warning, TEXT("URPGItemSaveData::SaveObjectNative: Trying to save an invalid Rarity level for item %s."), *RPGItem->GetItemFullName().ToString());
	}
	
	return Super::SaveObjectData(ObjectToSave);
}

bool URPGItemSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	URPGItem* RPGItem = Cast<URPGItem>(ObjectToLoad);
	if (!IsValid(RPGItem))
		return false;
	
	RPGItem->VisualDetails = SavedVisualDetails;
	RPGItem->RarityLevel = URPGInventoriesUtility::GetItemRarityByID(SavedRarityID);
	return Super::LoadObjectData(ObjectToLoad);
}
