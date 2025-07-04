// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EquipSlotKey.h"
#include "Base/ItemBase.h"
#include "Equipment.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogEquipmentSystem, All, All);

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

	UFUNCTION(BlueprintCallable)
	bool TryEquipItemToAvailableSlot(UItemBase* Item, const int32 SwapPriority = 0, bool bAllowSwapWithInventoryItem = true, bool bNotifyEvent = true);
	
	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, int32 SlotIndex, bool bAllowSwapWithInventoryItem = true, bool bNotifyEvent = true);
	
	UFUNCTION(BlueprintCallable)
	bool TryUnequipItem(UItemBase* Item, bool bNotifyEvent = true);
	
	UFUNCTION(BlueprintCallable)
	TSet<UItemBase*> GetEquippedItems();

	UFUNCTION(BlueprintCallable)
	void ClearEquipment();

	UFUNCTION(BlueprintPure)
	bool AreAllSlotsOfKeyOccupied(const UEquipSlotKey* TargetSlotKey) const;

	UFUNCTION(BlueprintCallable)
	UItemBase* FindEquippedItem(const UEquipSlotKey* TargetSlotKey) const;
	
	UFUNCTION(BlueprintCallable)
	UItemBase* FindEquippedItemAtIndex(const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const;

protected:
	virtual void BeginPlay() override;

	bool IsSlotKeyValid(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, int32 SlotIndex) const;

	static bool HasSameSlotKey(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey);

	bool IsSlotKeyEmpty(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, int32 SlotIndex) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnEquipItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item);

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequipItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item);
	
private:
	void EquipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory = true, const bool bNotifyEvent = true);

	void UnequipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, int32 SlotIndex, const bool bNotifyEvent = true);
	
	void AddItemInEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, int32 NewSlotIndex);

	void RemoveItemFromEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex);

	void ChangeItemEquipSlot(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex);
	
	bool TrySwitchEquippedItems(UItemBase* ItemA, UItemBase* ItemB);

	bool TrySwapWithInventoryItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, int32 SlotIndex, UItemBase* EquippedItem);
		
	UFUNCTION()
	void OnItemAddedToAnyInventory(UItemBase* Item, UInventoryBase* Inventory);
};
