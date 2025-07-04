// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGInventory/Data/RPGGearItemData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGItemsGeneration/RPGItemsGenerator.h"
#include "RPGItemsGeneration/Data/Generations/RPGGearItemGenerationData.h"
#include "StatsSystem/StatsBridgeBase.h"
#include "StatsSystem/Data/SpecialStatData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGItemsGeneration/Data/Generations/RPGGearItemGenerationSet.h"
#include "StatsSystem/BridgeStatOperation.h"
#include "RPGFactory.generated.h"

UCLASS()
class RPGSYSTEM_API URPGFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a generic RPG item instance based on the provided item data and generation data.
	 * @param WorldContextObject - The world context for spawning the item.
	 * @param Data - The item data asset to use for creation.
	 * @param GenerationData - The generation data for the item.
	 * @param bBuildItem - If true, the item will be fully built.
	 * @return Pointer to the created URPGItem instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGItem* CreateRPGGenericItem(UObject* WorldContextObject, URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem = true);

	/**
	 * Creates multiple generic RPG item instances based on the provided item data and generation data.
	 * @param WorldContextObject - The world context for spawning the items.
	 * @param Data - The item data asset to use for creation.
	 * @param GenerationData - The generation data for the items.
	 * @param OutOverflow - Output parameter for overflow quantity.
	 * @param bBuildItem - If true, the items will be fully built.
	 * @param Quantity - Number of items to create.
	 * @return Pointer to the created URPGItem instance(s).
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGItem* CreateRPGGenericItems(UObject* WorldContextObject, URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, int32& OutOverflow, const bool bBuildItem = true, const int32 Quantity = 1);

	/**
	 * Creates a gear item instance based on the provided gear data and generation data.
	 * @param WorldContextObject - The world context for spawning the gear item.
	 * @param Data - The gear item data asset to use for creation.
	 * @param GenerationData - The generation data for the gear item.
	 * @param bBuildItem - If true, the gear item will be fully built.
	 * @return Pointer to the created URPGGearItem instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGGearItem* CreateRPGGearItem(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem = true);

	/**
	 * Creates a gear item instance using a generation set.
	 * @param WorldContextObject - The world context for spawning the gear item.
	 * @param Data - The gear item data asset to use for creation.
	 * @param GenerationSet - The generation set to use for the gear item.
	 * @return Pointer to the created URPGGearItem instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|RPGSystem|Factories")
	static URPGGearItem* CreateRPGGearItemWithGenerationSet(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemGenerationSet* GenerationSet);

	/**
	 * Creates a gear item instance with fixed stats based on the provided generation data.
	 * @param WorldContextObject - The world context for spawning the gear item.
	 * @param Data - The gear item data asset to use for creation.
	 * @param GenerationData - The fixed stats generation data for the gear item.
	 * @return Pointer to the created URPGGearItem instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGGearItem* CreateRPGGearItemWithFixedStats(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemFixedStatsGenerationData* GenerationData);

	/**
	 * Creates a stat operation for the specified item.
	 * @param Operation - The stat operation to use as a base.
	 * @param Item - The item to apply the operation to.
	 * @return Pointer to the created UStatOperation instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Factories")
	static UStatOperation* CreateItemStatOperation(UStatOperation* Operation, URPGItem* Item);

	/**
	 * Creates a bridge stat operation for the specified bridge and stats.
	 * @param Bridge - The stats bridge to use.
	 * @param InCoreStatData - The core stat data to use.
	 * @param SpecialStat - The special stat data to use.
	 * @param OperationClass - The class of the bridge stat operation to create.
	 * @return Pointer to the created UBridgeStatOperation instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Factories")
	static UBridgeStatOperation* CreateBridgeStatOperation(UStatsBridgeBase* Bridge, UCoreStatData* InCoreStatData, USpecialStatData* SpecialStat, const TSubclassOf<UBridgeStatOperation> OperationClass);
};
