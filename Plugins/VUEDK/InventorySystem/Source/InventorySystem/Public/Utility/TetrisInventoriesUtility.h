// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory/TetrisInventoriesManager.h"
#include "TetrisInventory/TetrisInventoryEventsHandlerSubsystem.h"
#include "TetrisInventory/TetrisItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TetrisInventoriesUtility.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UTetrisInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Pointer to the current TetrisInventoryEventsHandlerSubsystem instance.
	 */
	static UTetrisInventoryEventsHandlerSubsystem* EventsHandlerSubsystem;
	
	/**
	 * Initializes the TetrisInventoriesUtility with the given EventsHandlerSubsystem.
	 * @param InEventsHandlerSubsystem The events handler subsystem to use.
	 */
	static void Init(UTetrisInventoryEventsHandlerSubsystem* InEventsHandlerSubsystem);
	
	/**
	 * Returns the TetrisInventoriesManager instance.
	 * @return Pointer to the UTetrisInventoriesManager.
	 */
	UFUNCTION(BlueprintPure, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static UTetrisInventoriesManager* GetTetrisInventoriesManager();

	/**
	 * Attempts to transfer an item to another Tetris inventory.
	 * @param Item The item to transfer.
	 * @param DestinationInventory The inventory to transfer the item to.
	 * @return True if the transfer was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory);
	
	/**
	 * Attempts to transfer an item to a specific position in another Tetris inventory.
	 * @param Item The item to transfer.
	 * @param DestinationInventory The inventory to transfer the item to.
	 * @param DestinationPosition The position in the destination inventory.
	 * @return True if the transfer was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition);

	/**
	 * Attempts to switch two items between Tetris inventories.
	 * @param ItemA The first item to switch.
	 * @param ItemB The second item to switch.
	 * @return True if the switch was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB);

	/**
	 * Requests to start dragging an item (e.g., for UI drag-and-drop).
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestDrag();

	/**
	 * Requests to drop the currently dragged item.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestDrop();

	/**
	 * Requests to rotate the currently dragged item.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestRotate();

	/**
	 * Requests to equip or unequip the currently selected item.
	 * @param SwapPriority The priority for swapping (default: 0).
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestEquipUnequip(const int32 SwapPriority = 0);

	/**
	 * Requests to transfer the currently selected item to the target inventory.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestTransferItemToTargetInventory();

	/**
	 * Requests to discard the currently selected item.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestDiscard();

	/**
	 * Requests to split the currently selected item stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestSplit();

	/**
	 * Requests to cancel the current split operation.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestCancelSplit();

	/**
	 * Checks if the player is currently splitting an item stack.
	 * @return True if splitting, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool IsSplitting();

private:
	static bool Check();
};
