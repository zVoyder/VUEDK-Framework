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

	UFUNCTION(BlueprintPure)
	URPGRarityLevelData* GetItemRarityByID(const FGuid RarityID) const;

	UFUNCTION(BlueprintPure)
	UCoreStatData* GetItemStatByID(const FGuid StatID) const;
};
