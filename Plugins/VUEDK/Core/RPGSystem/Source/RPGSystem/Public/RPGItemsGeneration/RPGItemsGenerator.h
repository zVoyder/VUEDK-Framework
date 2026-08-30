// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/FixedStatData.h"
#include "Data/Generations/RPGGearItemFixedStatsGenerationData.h"
#include "Data/Generations/RPGGearItemGenerationData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGInventory/Items/RPGItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/RPGRegistrySubsystem.h"
#include "RPGItemsGenerator.generated.h"

UCLASS()
class RPGSYSTEM_API URPGItemsGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<URPGRegistrySubsystem> RegistrySubsystem;
	
public:
	static void Init(URPGRegistrySubsystem* InRegistrySubsystem);
	
	/**
	 * Tries to generate a generic RPG item using the provided generation data.
	 * @param Item The RPG item to generate.
	 * @param GenerationData The generation data to use for item creation.
	 * @return True if the item was successfully generated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData);
	
	/**
	 * Tries to generate a gear RPG item using the provided generation data.
	 * @param GearItem The gear item to generate.
	 * @param GenerationData The generation data to use for gear item creation.
	 * @return True if the gear item was successfully generated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData);

	/**
	 * Tries to generate a gear RPG item with fixed stats using the provided generation data.
	 * @param GearItem The gear item to generate.
	 * @param GenerationData The fixed stats generation data to use for gear item creation.
	 * @return True if the gear item was successfully generated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|RPGItemsGenerator")
	static bool TryGenerateRPGGearItemWithFixedStats(URPGGearItem* GearItem, const URPGGearItemFixedStatsGenerationData* GenerationData);	

private:
	static FRPGItemVisualDetails GenerateVisualDetails(const URPGItemVisualSetData* Set);

	static URPGRarityLevelData* GenerateRarityLevel();
	
	static void GenerateItemStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData);
	
	static void GenerateItemStatsModifiersWithFixedStats(const URPGGearItem* Item, const TArray<FFixedStat>& Stats);

	static bool Check();
};
