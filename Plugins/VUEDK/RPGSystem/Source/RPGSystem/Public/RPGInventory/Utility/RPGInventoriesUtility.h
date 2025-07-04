// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGInventory/RPGInventoriesManager.h"
#include "RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGInventory/RPGInventoryCompareHandlerSubsystem.h"
#include "RPGInventoriesUtility.generated.h"

UCLASS()
class RPGSYSTEM_API URPGInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static URPGInventoryCompareHandlerSubsystem* CompareHandlerSubsystem;
	
public:
	static void Init(URPGInventoryCompareHandlerSubsystem* InCompareHandlerSubsystem);
	
	static UCoreStatData* GetItemStatByID(const FGuid StatID);
	
	static URPGRarityLevelData* GetItemRarityByID(const FGuid RarityID);

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static URPGInventoriesManager* GetRPGInventoriesManager();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static void SetTargetEquipmentForCompare(UEquipment* InComparingEquipment);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static void SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static void RequestCompare(const bool bCompare);

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static bool IsComparing();

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static UEquipment* GetTargetComparingEquipment();
	
	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static URPGGearItem* GetComparingItem();

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|RPGInventoriesUtility")
	static UEquipSlotKey* GetTargetComparingEquipSlotKey();
};
