// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/RPGInventoryCompareHandlerSubsystem.h"
#include "Subsystems/RPGRegistrySubsystem.h"
#include "RPGInventoriesUtility.generated.h"

UCLASS()
class RPGSYSTEM_API URPGInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<URPGRegistrySubsystem> RPGRegistrySubsystem;
	static TWeakObjectPtr<URPGInventoryCompareHandlerSubsystem> CompareHandlerSubsystem;
	
public:
	static void Init(URPGRegistrySubsystem* InRPGRegistrySubsystem);
	
	static void SetCompareHandler(URPGInventoryCompareHandlerSubsystem* InCompareHandlerSubsystem);
	
	/**
	 * Retrieves the stat data for an item by its unique identifier.
	 * @param StatID The unique identifier of the stat.
	 * @return Pointer to the UCoreStatData if found, nullptr otherwise.
	 */
	static UCoreStatData* GetItemStatByID(const FGuid StatID);
	
	/**
	 * Retrieves the rarity data for an item by its unique identifier.
	 * @param RarityID The unique identifier of the rarity.
	 * @return Pointer to the URPGRarityLevelData if found, nullptr otherwise.
	 */
	static URPGRarityLevelData* GetItemRarityByID(const FGuid RarityID);

	/**
	 * Sets the target equipment to be used for comparison.
	 * @param InComparingEquipment The equipment to compare.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|RPGInventoriesUtility")
	static void SetTargetEquipmentForCompare(UEquipment* InComparingEquipment);
	
	/**
	 * Sets the target equipment slot key to be used for comparison.
	 * @param EquipSlotKey The equipment slot key to compare.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|RPGInventoriesUtility")
	static void SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey);
	
	/**
	 * Requests a comparison operation.
	 * @param bCompare If true, initiates the comparison; if false, cancels it.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|RPGInventoriesUtility")
	static void RequestCompare(const bool bCompare);

	/**
	 * Checks if a comparison is currently in progress.
	 * @return true if comparing, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|RPGInventoriesUtility")
	static bool IsComparing();

	/**
	 * Gets the equipment currently being compared.
	 * @return Pointer to the UEquipment being compared, or nullptr if none.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|RPGInventoriesUtility")
	static UEquipment* GetTargetComparingEquipment();
	
	/**
	 * Gets the gear item currently being compared.
	 * @return Pointer to the URPGGearItem being compared, or nullptr if none.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|RPGInventoriesUtility")
	static URPGGearItem* GetComparingItem();

	/**
	 * Gets the equipment slot key currently being compared.
	 * @return Pointer to the UEquipSlotKey being compared, or nullptr if none.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|RPGInventoriesUtility")
	static UEquipSlotKey* GetTargetComparingEquipSlotKey();
};
