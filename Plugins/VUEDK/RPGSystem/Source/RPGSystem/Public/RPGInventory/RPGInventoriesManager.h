// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemsGeneration/Data/ItemsStatsData.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "RPGItemsGeneration/Data/Rarities/RPGItemsRaritiesData.h"
#include "TetrisInventory/TetrisInventoriesManager.h"
#include "RPGInventoriesManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGSYSTEM_API URPGInventoriesManager : public UTetrisInventoriesManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UItemsStatsData* ItemsStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPGItemsRaritiesData* ItemsRarities;
	
public:
	URPGInventoriesManager();

	/**
	 * Returns the rarity data for an item by its unique identifier.
	 * @param RarityID - The unique identifier of the rarity.
	 * @return Pointer to the URPGRarityLevelData if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	URPGRarityLevelData* GetItemRarityByID(const FGuid RarityID) const;

	/**
	 * Returns the stat data for an item by its unique identifier.
	 * @param StatID - The unique identifier of the stat.
	 * @return Pointer to the UCoreStatData if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	UCoreStatData* GetItemStatByID(const FGuid StatID) const;
};
