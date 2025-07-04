// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory.h"
#include "Base/InventoriesManager.h"
#include "TetrisInventoriesManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UTetrisInventoriesManager : public UInventoriesManager
{
	GENERATED_BODY()

public:
	UTetrisInventoriesManager();
	
	/**
	 * Attempts to transfer an item to another Tetris inventory.
	 * @param Item The item to transfer.
	 * @param DestinationInventory The inventory to transfer the item to.
	 * @return True if the transfer was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory);
	
	/**
	 * Attempts to transfer an item to a specific position in another Tetris inventory.
	 * @param Item The item to transfer.
	 * @param DestinationInventory The inventory to transfer the item to.
	 * @param DestinationPosition The position in the destination inventory.
	 * @return True if the transfer was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition);
	
	/**
	 * Attempts to switch two items between Tetris inventories.
	 * @param ItemA The first item to switch.
	 * @param ItemB The second item to switch.
	 * @return True if the switch was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB);
};
