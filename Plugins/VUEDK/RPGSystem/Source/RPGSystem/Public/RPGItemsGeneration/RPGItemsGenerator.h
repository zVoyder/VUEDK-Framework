// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/FixedStatData.h"
#include "Data/Generations/RPGGearItemFixedStatsGenerationData.h"
#include "Data/Generations/RPGGearItemGenerationData.h"
#include "Data/Rarities/RPGItemsRaritiesData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGInventory/Items/RPGItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGItemsGenerator.generated.h"

UCLASS()
class RPGSYSTEM_API URPGItemsGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGGearItemWithFixedStats(URPGGearItem* GearItem, const URPGGearItemFixedStatsGenerationData* GenerationData);	

private:
	static FRPGItemVisualDetails GenerateVisualDetails(const URPGItemVisualSetData* Set);
	
	static URPGRarityLevelData* GenerateRarityLevel(const URPGItemsRaritiesData* Set);
	
	static void GenerateItemStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData);

	static void GenerateItemStatsModifiersWithFixedStats(const URPGGearItem* Item, const TArray<FFixedStat>& Stats);
};
