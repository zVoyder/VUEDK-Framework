// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentSystem/Equipment.h"
#include "InventoryBase.generated.h"

struct FItemSaveData;
class UItemsRegistry;
class UItemBase;
class UItemDataBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemAdded,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemConsumed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemUsed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemRemoved,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryCleared
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryModified
);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom))
class INVENTORYSYSTEM_API UInventoryBase : public UActorComponent
{
	GENERATED_BODY()

	friend UItemBase;
	
public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnInventoryCleared OnInventoryCleared;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnInventoryModified OnInventoryModified;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemAdded OnAnyItemAdded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemUsed OnAnyItemUsed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemConsumed OnAnyItemConsumed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyItemRemoved OnAnyItemRemoved;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1", ToolTip = "The maximum number of items that can be stored in the inventory."))
	int32 Capacity = 16;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsMainInventory = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", EditCondition = "bUseWeight"))
	float WeightMaxCapacity;

protected:
	UPROPERTY()
	TSet<UItemBase*> Items;

private:
	UPROPERTY()
	TObjectPtr<UEquipment> RelatedEquipment;
	float CurrentWeight;

public:
	UInventoryBase();
	
	/**
	 * Links this inventory to the specified equipment.
	 * @param Equipment The equipment to link.
	 */
	UFUNCTION(BlueprintCallable)
	void LinkEquipment(UEquipment* Equipment);

	/**
	 * Unlinks the currently linked equipment from this inventory.
	 */
	UFUNCTION(BlueprintCallable)
	void UnlinkEquipment();

	/**
	 * Returns the currently linked equipment.
	 * @return Pointer to the linked UEquipment.
	 */
	UFUNCTION(BlueprintPure)
	UEquipment* GetEquipment() const;

	/**
	 * Adds a new item to the inventory.
	 * @param ItemData The item data to add.
	 * @param OutOverflow The number of items that could not be added due to overflow.
	 * @param ItemQuantity The quantity to assign to the new item (default: 1), this is not the amount to add, but the quantity of the new item to create.
	 * @return Pointer to the added UItemBase.
	 */
	UFUNCTION(BlueprintCallable)
	UItemBase* AddNewItem(UItemDataBase* ItemData, int32& OutOverflow, const int32 ItemQuantity = 1);

	/**
	 * Tries to add an item to the inventory.
	 * @param Item The item to add.
	 * @return True if the item was added, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemBase* Item);

	/**
	 * Removes an item from the inventory by its data Id.
	 * @param ItemData The item data to remove.
	 * @return True if the item was removed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool RemoveItemByDataId(UItemDataBase* ItemData);

	/**
	 * Removes the specified item from the inventory.
	 * @param Item The item to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemBase* Item);

	/**
	 * Consumes a specified amount of an item from the inventory.
	 * @param ItemData The item data to consume.
	 * @param AmountToConsume The amount to consume (default: 1).
	 * @param bForceConsume Whether to force consumption even if not enough items are present (default: false).
	 */
	UFUNCTION(BlueprintCallable)
	void ConsumeItem(UItemDataBase* ItemData, const int32 AmountToConsume = 1, const bool bForceConsume = false) const;

	/**
	 * Clears all items from the inventory.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearInventory();

	/**
	 * Finds the first item in the inventory that matches the specified item data.
	 * @param ItemData The item data to find.
	 * @param bSortByQuantity Whether to sort the found items by quantity in ascending order.
	 * @return Pointer to the found UItemBase, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable)
	UItemBase* Find(const UItemDataBase* ItemData, const bool bSortByQuantity = true) const;

	/**
	 * Finds all items in the inventory that match the specified item data.
	 * @param ItemData The item data to find.
	 * @param bSortByQuantity Whether to sort the found items by quantity in ascending order.
	 * @return An array of UItemBase pointers representing the found items.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<UItemBase*> FindAll(const UItemDataBase* ItemData, bool bSortByQuantity = true) const;

	/**
	 * Checks if the inventory is full.
	 * @return True if the inventory is full, false otherwise.
	 */
	virtual bool IsFull() const;

	/**
	 * Checks if the inventory is completely full, meaning it has also all stacks filled to their maximum capacity.
	 * @return True if the inventory is completely full, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsCompletelyFull() const;

	/**
	 * Checks if the inventory is empty.
	 * @return True if the inventory is empty, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	virtual bool IsEmpty() const;

	/**
	 * Checks if the inventory can contain a specific item.
	 * @param ItemData The item data to check.
	 * @return True if the inventory can contain the item, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	virtual bool CanContainItem(const UItemDataBase* ItemData) const;

	/**
	 * Checks if the inventory can support additional weight.
	 * @param Weight The additional weight to check.
	 * @return True if the inventory can support the additional weight, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool CanSupportAdditionalWeight(float Weight) const;

	/**
	 * Checks if the inventory has an item of a specific type Id.
	 * @param ItemData The item data to check.
	 * @return True if the inventory has an item of the specified type Id, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasItemOfTypeId(const UItemDataBase* ItemData) const;

	/**
	 * Checks if the inventory has an item of a specific data Id.
	 * @param ItemData The item data to check.
	 * @return True if the inventory has an item of the specified data Id, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasItemOfDataId(const UItemDataBase* ItemData) const;

	/**
	 * Checks if the inventory has enough quantity of a specific item to consume.
	 * @param ItemData The item data to check.
	 * @param QuantityToConsume The quantity to consume.
	 * @return True if there is enough quantity to consume, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasEnoughQuantityToConsume(const UItemDataBase* ItemData, const int32 QuantityToConsume) const;

	/**
	 * Gets the item data from the registry by its Id.
	 * @param ItemDataID The Id of the item data to retrieve.
	 * @return Pointer to the UItemDataBase if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	UItemDataBase* GetItemDataFromRegistry(const FGuid ItemDataID) const;

	/**
	 * Retrieves all items in the inventory.
	 * @param bIncludeEquippedItems Whether to include equipped items from the linked equipment.
	 * @return An array of UItemBase pointers representing the items in the inventory.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UItemBase*> GetItems(const bool bIncludeEquippedItems = true);

	/**
	 * Gets the current weight of the inventory.
	 * @return The current weight of the inventory.
	 */
	UFUNCTION(BlueprintPure)
	double GetCurrentWeight() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/**
	 * Adds an item to the inventory list.
	 * @param Item The item to add.
	 */
	void AddItemToList(UItemBase* Item);

	/**
	 * Removes an item from the inventory list.
	 * @param Item The item to remove.
	 */
	void RemoveItemFromList(UItemBase* Item);

	/**
	 * Called when an item is added to the inventory.
	 * @param Item The item that was added.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnItemAdded(UItemBase* Item);

	/**
	 * Called when an item is removed from the inventory.
	 * @param Item The item that was removed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnItemRemoved(UItemBase* Item);

	/**
	 * Called when the inventory is cleared.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnClearedInventory();

private:
	void SetupMainInventory();
	
	bool TryIncreaseAvailableStackForNewItem(UItemDataBase* ItemData, const int32 Amount, UItemBase*& OutFoundItem, int32& OutOverflow);

	bool TryIncreaseAvailableStackForItem(UItemBase* Item) const;
	
	UItemBase* FindAvailableStackForNewItem(const UItemDataBase* ItemData) const;

	UItemBase* FindAvailableStackForItem(UItemBase* Item) const;
	
	void AddWeight(const float Weight);
	
	void RemoveWeight(const float Weight);

	UFUNCTION()
	void OnEquipmentChanged();
};
