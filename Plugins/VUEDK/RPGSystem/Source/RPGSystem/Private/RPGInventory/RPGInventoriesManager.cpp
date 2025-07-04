// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/RPGInventoriesManager.h"

URPGInventoriesManager::URPGInventoriesManager(): ItemsStats(nullptr),
                                                  ItemsRarities(nullptr)
{
}

URPGRarityLevelData* URPGInventoriesManager::GetItemRarityByID(const FGuid RarityID) const
{
	for (URPGRarityLevelData* RarityLevel : ItemsRarities->RarityLevels)
	{
		if (!IsValid(RarityLevel))
		{
			UE_LOG(LogTemp, Error, TEXT("URPGInventoriesManager::GetRarityByID: Found invalid rarity level in ItemsRarities."));
			continue;
		}
		
		if (RarityLevel->RarityID == RarityID)
			return RarityLevel;
	}

	return nullptr;
}

UCoreStatData* URPGInventoriesManager::GetItemStatByID(const FGuid StatID) const
{
	if (!ItemsStats)
		return nullptr;
	
	for (UCoreStatData* Stat : ItemsStats->Stats)
	{
		if (!IsValid(Stat))
		{
			UE_LOG(LogTemp, Error, TEXT("URPGInventoriesManager::GetStatByID: Found invalid stat in ItemsStats."));
			continue;
		}
		
		if (Stat->StatID == StatID)
			return Stat;
	}
	
	return nullptr;
}
