// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisSlot.h"
#include "Base/InventoryBase.h"
#include "TetrisInventory.generated.h"

class UTetrisItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemAddedToSlot,
	UTetrisItem*, Item,
	FIntPoint, SlotPosition
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemRemovedFromSlot,
	UTetrisItem*, Item,
	FIntPoint, SlotPosition
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnItemMoved,
	UTetrisItem*, Item,
	FIntPoint, OldPosition,
	FIntPoint, NewPosition
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FOnItemsSwitchedPositions,
	UTetrisItem*, ItemA,
	UTetrisItem*, ItemB,
	FIntPoint, ItemAOldPosition,
	FIntPoint, ItemBOldPosition,
	FIntPoint, ItemANewPosition,
	FIntPoint, ItemBNewPosition
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UTetrisInventory : public UInventoryBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemAddedToSlot OnItemAddedToSlot;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemRemovedFromSlot OnItemRemovedFromSlot;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemMoved OnItemMoved;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemsSwitchedPositions OnItemsSwitchedPositions;

	UPROPERTY(EditDefaultsOnly)
	FIntPoint GridSize;

private:
	TArray<TArray<UTetrisSlot*>> InvMatrix;

public:
	UTetrisInventory();

	virtual bool CanContainItem(const UItemDataBase* ItemData) const override;

	virtual bool IsFull() const override;

	/**
	 * Gets the slot at the specified position in the grid.
	 * @param SlotPosition The position of the slot in the grid.
	 * @return Pointer to the UTetrisSlot at the specified position, or nullptr if the position is invalid.
	 */
	UFUNCTION(BlueprintPure)
	UTetrisSlot* GetSlot(const FIntPoint SlotPosition) const;

	/**
	 * Gets the size of the grid.
	 * @return The size of the grid as an FIntPoint.
	 */
	UFUNCTION(BlueprintPure)
	FIntPoint GetGridSize() const;

	/**
	 * Checks if the specified item can occupy the slots in the grid.
	 * @param Size The size of the item in grid slots.
	 * @param StartSlotPosition The starting slot position in the grid.
	 * @param bOutNeedsRotation Output parameter indicating if the item needs rotation to fit.
	 * @param bCheckRotation Whether to check for rotation (default: false).
	 * @param IgnoreItem An item to ignore during the check (optional).
	 * @return True if the item can occupy the slots, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool CanOccupySlots(const FIntPoint Size, const FIntPoint StartSlotPosition, bool& bOutNeedsRotation, const bool bCheckRotation = false, const UTetrisItem* IgnoreItem = nullptr) const;

	/**
	 * Tries to add a new item at the specified slot position.
	 * @param ItemData The data of the item to add.
	 * @param StartSlotPosition The starting slot position in the grid.
	 * @return True if the item was added successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddNewItemAtSlots(UTetrisItemData* ItemData, const FIntPoint StartSlotPosition);

	/**
	 * Tries to add an existing item at the specified slot position.
	 * @param Item The item to add.
	 * @param StartSlotPosition The starting slot position in the grid.
	 * @return True if the item was added successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddItemAtSlots(UTetrisItem* Item, const FIntPoint StartSlotPosition);

	/**
	 * Tries to move an item to a new position in the grid, optionally rotating it.
	 * @param Item The item to move.
	 * @param NewPosition The new position in the grid.
	 * @param bTryRotate Whether to try rotating the item if it doesn't fit (default: false).
	 * @param IgnoreItem An item to ignore during the move (optional).
	 * @return True if the item was moved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bCallEvents"))
	bool TryMoveItem(UTetrisItem* Item, const FIntPoint NewPosition, const bool bTryRotate = false, const UTetrisItem* IgnoreItem = nullptr);

	/**
	 * Tries to switch the positions of two items in the grid.
	 * @param ItemA The first item to switch.
	 * @param ItemB The second item to switch.
	 * @return True if the items were switched successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TrySwitchItemsPositions(UTetrisItem* ItemA, UTetrisItem* ItemB);

protected:
	/**
	 * Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;

	/**
	 * Creates the save data object for this inventory.
	 * @return Pointer to the created USaveData object.
	 */
	virtual USaveData* CreateSaveDataObject_Implementation() override;

	/**
	 * Creates the inventory save data for this inventory.
	 * @param SaveData The base save data object.
	 * @param ItemsToSave The items to include in the save data.
	 * @return Pointer to the created USaveData object.
	 */
	virtual USaveData* CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave) override;

	/**
	 * Loads the inventory state from the provided save data.
	 * @param InventorySaveData The save data to load from.
	 */
	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	/**
	 * Called when an item is added to the inventory.
	 * @param Item The item that was added.
	 */
	virtual void OnItemAdded_Implementation(UItemBase* Item) override;

	/**
	 * Called when an item is removed from the inventory.
	 * @param Item The item that was removed.
	 */
	virtual void OnItemRemoved_Implementation(UItemBase* Item) override;

	/**
	 * Called when the inventory is cleared.
	 */
	virtual void OnClearedInventory_Implementation() override;

private:
	/**
	 * Constructs the grid for the Tetris inventory.
	 */
	void ConstructGrid();

	/**
	 * Checks if the given slot position is valid within the grid.
	 * @param SlotPosition The slot position to check.
	 * @return True if the slot position is valid, false otherwise.
	 */
	bool IsValidSlotPosition(const FIntPoint SlotPosition) const;

	/**
	 * Frees the slots starting from the given position and of the given size.
	 * @param StartPosition The starting position of the slots to free.
	 * @param Size The size of the area to free.
	 */
	void FreeSlots(const FIntPoint StartPosition, const FIntPoint Size);

	/**
	 * Occupies slots in the grid with the specified item.
	 * @param Item The item to occupy slots with.
	 * @param StartSlotPosition The starting slot position.
	 * @param bAddItemToList Whether to add the item to the inventory list (default: true).
	 */
	void OccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList = true);

	/**
	 * Tries to occupy slots in the grid with the specified item.
	 * @param Item The item to occupy slots with.
	 * @param StartSlotPosition The starting slot position.
	 * @param bTryRotate Whether to try rotating the item if it doesn't fit (default: false).
	 * @param IgnoreItem An item to ignore during the operation (optional).
	 * @param bAddItemToList Whether to add the item to the inventory list (default: true).
	 * @return True if the slots were successfully occupied, false otherwise.
	 */
	bool TryOccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bTryRotate = false, const UTetrisItem* IgnoreItem = nullptr, const bool bAddItemToList = true);

	/**
	 * Finds available slots in the grid for an item of the given size.
	 * @param Size The size of the item.
	 * @param OutStartSlotPosition Output parameter for the found start slot position.
	 * @param OutbNeedsRotation Output parameter indicating if rotation is needed.
	 * @return True if available slots were found, false otherwise.
	 */
	bool FindAvailableSlots(const FIntPoint Size, FIntPoint& OutStartSlotPosition, bool& OutbNeedsRotation) const;
};
