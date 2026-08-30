// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/SaveData/RPGGearItemSaveData.h"
#include "RPGSaveBridge.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

bool URPGGearItemSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const URPGGearItem* RPGGearItem = Cast<URPGGearItem>(ObjectToSave);
	if (!IsValid(RPGGearItem))
		return false;

	const URPGGearItemStatsContainer* GearStatsContainer = RPGGearItem->GearStatsContainer;
	if (!IsValid(GearStatsContainer))
		return false;
	
	for (const auto& StatModifier : GearStatsContainer->GetValues())
		SavedGearStats.Add(StatModifier.Key->StatId, StatModifier.Value);
	
	return Super::SaveObjectData(ObjectToSave);
}

bool URPGGearItemSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	const URPGGearItem* RPGGearItem = Cast<URPGGearItem>(ObjectToLoad);
	if (!IsValid(RPGGearItem))
		return false;

	URPGGearItemStatsContainer* GearStatsContainer = RPGGearItem->GearStatsContainer;
	if (!IsValid(GearStatsContainer))
		return false;
	
	for (const auto& Stats : SavedGearStats)
	{
		if (UCoreStatData* StatData = URPGInventoriesUtility::GetItemStatByID(Stats.Key); IsValid(StatData))
			GearStatsContainer->AddStat(StatData, Stats.Value);
		else
			UE_LOG(LogRPGSaveBridge, Warning, TEXT("URPGGearItemSaveData::LoadObjectNative: Failed to load RPGGearItem bonus stat with Id: %s"), *Stats.Key.ToString());
	}
	
	return Super::LoadObjectData(ObjectToLoad);
}
