// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EquipSlotKey.h"
#include "Base/ItemBase.h"
#include "Equipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyItemEquipped,
	UEquipSlotKey*, EquipSlotKey,
	int32, SlotIndex,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyItemUnequipped,
	UEquipSlotKey*, EquipSlotKey,
	int32, SlotIndex,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyItemEquippedNotify,
	UEquipSlotKey*, EquipSlotKey,
	int32, SlotIndex,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyItemUnequippedNotify,
	UEquipSlotKey*, EquipSlotKey,
	int32, SlotIndex,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnAnyItemEquipSlotChanged,
	UEquipSlotKey*, EquipSlotKey,
	UItemBase*, Item,
	int32, NewSlotIndex,
	int32, OldSlotIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnAnyItemSwapWithInventoryItem,
	UEquipSlotKey*, EquipSlotKey,
	UItemBase*, Item,
	UItemBase*, InventoryItem,
	int32, SlotIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(
	FOnItemsSwitchedSlots,
	UEquipSlotKey*, EquipSlotKey,
	UItemBase*, ItemA,
	UItemBase*, ItemB,
	int32, OldSlotIndexItemA,
	int32, OldSlotIndexItemB,
	int32, NewSlotIndexItemA,
	int32, NewSlotIndexItemB
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEquipChanged
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEquipmentCleared
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UEquipment : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when an item is notified to be equipped.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemEquippedNotify OnAnyItemEquipNotify;
	/**
	 * @brief Called when an item is notified to be unequipped.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemUnequippedNotify OnAnyItemUnequipNotify;
	/**
	 * @brief Called when an item is equipped.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemEquipped OnAnyItemEquipped;
	/**
	 * @brief Called when an item is unequipped.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemUnequipped OnAnyItemUnequipped;
	/**
	 * @brief Called when an item is equipped and has changed its slot with an empty one.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemEquipSlotChanged OnAnyItemEquipSlotChanged;
	/**
	 * @brief Called when two items have switched slots.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemsSwitchedSlots OnItemsSwitchedSlots;
	/**
	 * @brief Called when an item is swapped with an item in the inventory.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemSwapWithInventoryItem OnAnyItemSwapWithInventoryItem;
	/**
	 * @brief Generic event called when equipment has changed its state.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEquipChanged OnEquipChanged;
	/**
	 * @brief Called when equipment is cleared.
	 */
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEquipmentCleared OnEquipmentCleared;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<UEquipSlotKey*, int32> SlotLimits;

private:
	TMap<FName, TMap<int32, UItemBase*>> EquipSlots;

public:
	UEquipment();

	/**
	 * @brief Tries to equip an item to an available slot.
	 * @param Item The item to equip.
	 * @param SwapPriority The priority for swapping (default: 0).
	 * @param bAllowSwapWithInventoryItem Whether to allow swapping with an inventory item (default: true).
	 * @param bNotifyEvent Whether to notify events (default: true).
	 * @return True if the item was equipped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryEquipItemToAvailableSlot(UItemBase* Item, const int32 SwapPriority = 0, bool bAllowSwapWithInventoryItem = true, bool bNotifyEvent = true);
	
	/**
	 * @brief Tries to equip an item to a specific slot.
	 * @param Item The item to equip.
	 * @param TargetSlotKey The target slot key.
	 * @param SlotIndex The index of the slot.
	 * @param bAllowSwapWithInventoryItem Whether to allow swapping with an inventory item (default: true).
	 * @param bNotifyEvent Whether to notify events (default: true).
	 * @return True if the item was equipped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, int32 SlotIndex, bool bAllowSwapWithInventoryItem = true, bool bNotifyEvent = true);
	
	/**
	 * @brief Tries to unequip an item.
	 * @param Item The item to unequip.
	 * @param bNotifyEvent Whether to notify events (default: true).
	 * @return True if the item was unequipped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryUnequipItem(UItemBase* Item, bool bNotifyEvent = true);
	
	/**
	 * @brief Gets all equipped items.
	 * @return A set of pointers to the equipped UItemBase items.
	 */
	UFUNCTION(BlueprintCallable)
	TSet<UItemBase*> GetEquippedItems();

	/**
	 * @brief Clears the equipment, unequipping all items.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearEquipment();

	/**
	 * @brief Checks if all slots of the specified key are occupied.
	 * @param TargetSlotKey The slot key to check.
	 * @return True if all slots are occupied, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool AreAllSlotsOfKeyOccupied(const UEquipSlotKey* TargetSlotKey) const;

	/**
	 * Finds the equipped item for the specified slot key.
	 * @param TargetSlotKey The slot key to search for.
	 * @return Pointer to the equipped UItemBase, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable)
	UItemBase* FindEquippedItem(const UEquipSlotKey* TargetSlotKey) const;
	
	/**
	 * Finds the equipped item at the specified slot index for the given slot key.
	 * @param TargetSlotKey The slot key to search for.
	 * @param SlotIndex The index of the slot to search in.
	 * @return Pointer to the equipped UItemBase, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable)
	UItemBase* FindEquippedItemAtIndex(const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const;

protected:
	virtual void BeginPlay() override;

	/**
	 * Checks if the slot key is valid for the given item and slot index.
	 * @param Item The item to check.
	 * @param TargetSlotKey The slot key to validate.
	 * @param SlotIndex The slot index to validate.
	 * @return True if the slot key is valid, false otherwise.
	 */
	bool IsSlotKeyValid(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, int32 SlotIndex) const;

	/**
	 * Checks if the item has the same slot key as the target.
	 * @param Item The item to check.
	 * @param TargetSlotKey The slot key to compare.
	 * @return True if the slot keys are the same, false otherwise.
	 */
	static bool HasSameSlotKey(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey);

	/**
	 * Checks if the slot key is empty for the given item and slot index.
	 * @param Item The item to check.
	 * @param TargetSlotKey The slot key to validate.
	 * @param SlotIndex The slot index to validate.
	 * @return True if the slot key is empty, false otherwise.
	 */
	bool IsSlotKeyEmpty(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, int32 SlotIndex) const;

	/**
	 * Called when an item is equipped.
	 * @param EquipSlotKey The slot key where the item is equipped.
	 * @param Item The item that was equipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEquipItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item);

	/**
	 * Called when an item is unequipped.
	 * @param EquipSlotKey The slot key from which the item is unequipped.
	 * @param Item The item that was unequipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnUnequipItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item);
	
private:
	/**
	 * Equips the specified item in the given slot.
	 * @param Item The item to equip.
	 * @param EquipSlotKey The slot key to equip the item in.
	 * @param SlotIndex The index of the slot.
	 * @param bRemoveFromInventory Whether to remove the item from the inventory (default: true).
	 * @param bNotifyEvent Whether to notify events (default: true).
	 */
	void EquipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory = true, const bool bNotifyEvent = true);

	/**
	 * Unequips the specified item from the given slot.
	 * @param Item The item to unequip.
	 * @param EquipSlotKey The slot key to unequip the item from.
	 * @param SlotIndex The index of the slot.
	 * @param bNotifyEvent Whether to notify events (default: true).
	 */
	void UnequipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, int32 SlotIndex, const bool bNotifyEvent = true);
	
	/**
	 * Adds an item to the specified equip slot.
	 * @param Item The item to add.
	 * @param EquipSlotKey The slot key to add the item to.
	 * @param NewSlotIndex The index of the slot.
	 */
	void AddItemInEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, int32 NewSlotIndex);

	/**
	 * Removes an item from the specified equip slot.
	 * @param Item The item to remove.
	 * @param EquipSlotKey The slot key to remove the item from.
	 * @param SlotIndex The index of the slot.
	 */
	void RemoveItemFromEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex);

	/**
	 * Changes the equip slot of the specified item.
	 * @param Item The item to move.
	 * @param EquipSlotKey The slot key to move the item to.
	 * @param NewSlotIndex The new slot index.
	 */
	void ChangeItemEquipSlot(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex);
	
	/**
	 * Tries to switch two equipped items.
	 * @param ItemA The first item to switch.
	 * @param ItemB The second item to switch.
	 * @return True if the switch was successful, false otherwise.
	 */
	bool TrySwitchEquippedItems(UItemBase* ItemA, UItemBase* ItemB);

	/**
	 * Tries to swap an equipped item with an inventory item.
	 * @param Item The equipped item to swap.
	 * @param TargetSlotKey The target slot key.
	 * @param SlotIndex The slot index.
	 * @param EquippedItem The inventory item to swap with.
	 * @return True if the swap was successful, false otherwise.
	 */
	bool TrySwapWithInventoryItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, int32 SlotIndex, UItemBase* EquippedItem);
		
	/**
	 * Called when an item is added to any inventory.
	 * @param Item The item that was added.
	 * @param Inventory The inventory to which the item was added.
	 */
	UFUNCTION()
	void OnItemAddedToAnyInventory(UItemBase* Item, UInventoryBase* Inventory);
};
