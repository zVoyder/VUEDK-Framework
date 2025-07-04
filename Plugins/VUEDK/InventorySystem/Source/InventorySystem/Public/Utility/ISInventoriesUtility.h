// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/InventoriesManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISInventoriesUtility.generated.h"

class UInventoryBase;

UCLASS()
class INVENTORYSYSTEM_API UISInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UInventoriesManager* InventoriesManager;
	static UInventoryBase* MainInventory;
	static UInventoryBase* TargetInventory;

public:
	static void Init(UInventoriesManager* Manager);

	/**
	 * @brief Sets the main inventory for the utility functions, used to perform equip and unequip actions so the item knows where to go.
	 * @param Inventory The inventory to set as main.
	 */
	static void SetMainInventory(UInventoryBase* Inventory);
	
	/**
	 * @brief Sets the target inventory for the utility functions, used to perform transfer actions so the item knows where to go.
	 * @param Inventory The inventory to set as target.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tetris Inventory System")
	static void SetTargetInventory(UInventoryBase* Inventory);

	/**
	 * @brief Gets the target inventory set by SetTargetInventory.
	 * @return The target inventory.
	 */
	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	static UInventoryBase* GetTargetInventory();

	/**
	 * @brief Gets the main inventory set by SetMainInventory.
	 * @return The main inventory.
	 */
	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	static UInventoryBase* GetMainInventory();

	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	static UInventoriesManager* GetInventoriesManager();

	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	static UItemsRegistry* GetRegistry();

	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	static bool IsItemInRegistry(const UItemDataBase* ItemData);

	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	static UItemDataBase* GetItemDataFromRegistry(const FGuid& ItemDataID);

	/**
	 * @brief Drops an item using the drop operation defined in the settings.
	 * @param Item The item to drop.
	 * @return True if the item was dropped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tetris Inventory System")
	static bool DropItemWithOperation(UItemBase* Item);
	
private:
	static bool Check();
};
