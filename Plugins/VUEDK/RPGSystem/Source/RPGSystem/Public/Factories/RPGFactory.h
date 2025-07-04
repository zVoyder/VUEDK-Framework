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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGItem* CreateRPGGenericItem(UObject* WorldContextObject, URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem = true);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGItem* CreateRPGGenericItems(UObject* WorldContextObject, URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, int32& OutOverflow, const bool bBuildItem = true, const int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGGearItem* CreateRPGGearItem(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem = true);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|RPGSystem|Factories")
	static URPGGearItem* CreateRPGGearItemWithGenerationSet(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemGenerationSet* GenerationSet);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "bBuildItem"), Category = "VUEDK|RPGSystem|Factories")
	static URPGGearItem* CreateRPGGearItemWithFixedStats(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemFixedStatsGenerationData* GenerationData);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Factories")
	static UStatOperation* CreateItemStatOperation(UStatOperation* Operation, URPGItem* Item);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Factories")
	static UBridgeStatOperation* CreateBridgeStatOperation(UStatsBridgeBase* Bridge, UCoreStatData* InCoreStatData, USpecialStatData* SpecialStat, const TSubclassOf<UBridgeStatOperation> OperationClass);
};
