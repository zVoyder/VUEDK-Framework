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
	/**
	 * Tries to generate a generic RPG item using the provided generation data.
	 * @param Item - The RPG item to generate.
	 * @param GenerationData - The generation data to use for item creation.
	 * @return True if the item was successfully generated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData);
	
	/**
	 * Tries to generate a gear RPG item using the provided generation data.
	 * @param GearItem - The gear item to generate.
	 * @param GenerationData - The generation data to use for gear item creation.
	 * @return True if the gear item was successfully generated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData);

	/**
	 * Tries to generate a gear RPG item with fixed stats using the provided generation data.
	 * @param GearItem - The gear item to generate.
	 * @param GenerationData - The fixed stats generation data to use for gear item creation.
	 * @return True if the gear item was successfully generated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGGearItemWithFixedStats(URPGGearItem* GearItem, const URPGGearItemFixedStatsGenerationData* GenerationData);	

private:
	/**
	 * Generates the visual details for an RPG item from the provided visual set data.
	 * @param Set - The visual set data to use.
	 * @return The generated FRPGItemVisualDetails struct.
	 */
	static FRPGItemVisualDetails GenerateVisualDetails(const URPGItemVisualSetData* Set);
	
	/**
	 * Generates the rarity level for an RPG item from the provided rarities data set.
	 * @param Set - The rarities data set to use.
	 * @return Pointer to the generated URPGRarityLevelData, or nullptr if not found.
	 */
	static URPGRarityLevelData* GenerateRarityLevel(const URPGItemsRaritiesData* Set);
	
	/**
	 * Generates stat modifiers for a gear item using the provided generation data.
	 * @param Item - The gear item to modify.
	 * @param GenerationData - The generation data to use for stat modifiers.
	 */
	static void GenerateItemStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData);

	/**
	 * Generates stat modifiers for a gear item using fixed stats.
	 * @param Item - The gear item to modify.
	 * @param Stats - The array of fixed stats to apply.
	 */
	static void GenerateItemStatsModifiersWithFixedStats(const URPGGearItem* Item, const TArray<FFixedStat>& Stats);
};
