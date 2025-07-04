// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/ItemSaveData.h"
#include "EquipmentSystem/Data/EquipSlotKey.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

class UClassicInventory;
class UItemDataBase;
class UEquipment;
class AItemDropActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemAdded,
	UItemBase*, Item,
	UInventoryBase*, Inventory
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemUsed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemConsumed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemRemoved,
	UItemBase*, Item,
	UInventoryBase*, Inventory
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemEquipped,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemUnequipped,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemDropped,
	UItemBase*, Item,
	AItemDropActor*, ItemDropActor
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemDroppedNotify,
	UItemBase*, Item,
	AItemDropActor*, ItemDropActor
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemQuantityChanged,
	UItemBase*, Item,
	int32, Quantity
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemMeshLoaded,
	UStaticMesh*, Mesh
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API UItemBase : public UObject
{
	GENERATED_BODY()

	friend class UInventoryBase;

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemAdded OnItemAdded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemUsed OnItemUsed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemConsumed OnItemConsumed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemRemoved OnItemRemoved;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemEquipped OnItemEquipped;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemUnequipped OnItemUnequipped;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemDropped OnItemDropped;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemDroppedNotify OnItemDroppedNotify;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemQuantityChanged OnItemQuantityChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemMeshLoaded OnItemMeshLoaded;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryBase* RelatedInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEquipment* RelatedEquipment;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UItemDataBase* ItemData;
	UPROPERTY()
	TSoftObjectPtr<UStaticMesh> ItemMesh;

private:
	UPROPERTY()
	int32 EquipSlotIndex;
	UPROPERTY()
	int32 CurrentQuantity;
	UPROPERTY()
	UObject* ItemWorldContext;

public:
	UItemBase();
	
	FItemSaveData CreateItemBaseSaveData() const;

	void LoadItemBaseSaveData(UInventoryBase* LoadingInventory, const FItemSaveData ItemSaveData, bool& bOutHasBeenEquipped);

	/**
	 * Initializes the item with the given world context and item data.
	 * @param WorldContextObject The world context object.
	 * @param Data The item data to initialize with.
	 */
	virtual void Init(UObject* WorldContextObject, UItemDataBase* Data);

	/**
	 * Returns the item data associated with this item.
	 * @return Pointer to the UItemDataBase.
	 */
	UFUNCTION(BlueprintPure)
	UItemDataBase* GetItemData() const;

	/**
	 * Assigns this item to the specified inventory.
	 * @param Inventory The inventory to assign to.
	 */
	void AssignInventory(UInventoryBase* Inventory);

	/**
	 * Removes this item from its assigned inventory.
	 */
	void DeassignInventory();

	/**
	 * Sets the equip slot for this item.
	 * @param InEquipment The equipment to assign.
	 * @param SlotIndex The index of the equip slot.
	 */
	void SetEquipSlot(UEquipment* InEquipment, const int32 SlotIndex);

	/**
	 * Clears the equip slot for this item.
	 */
	void ClearEquipSlot();

	/**
	 * Checks if the item is currently equipped.
	 * @return True if the item is equipped, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEquipped() const;

	/**
	 * Attempts to drop the item into the world at the specified location and rotation.
	 * @param Location The location where the item should be dropped.
	 * @param Rotation The rotation of the item when dropped.
	 * @param OutItemDropActor Reference to the output item drop actor created.
	 * @param bNotify Whether to notify the item drop event (default: true).
	 * @return True if the drop was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryDrop(const FVector Location, const FRotator Rotation, AItemDropActor*& OutItemDropActor, const bool bNotify = true);

	/**
	 * Removes the item from its inventory.
	 */
	UFUNCTION(BlueprintCallable)
	void Remove();

	/**
	 * Uses the item, triggering any use-related events and logic.
	 */
	UFUNCTION(BlueprintCallable)
	void Use();

	/**
	 * Consumes the item, reducing its quantity and triggering consumption events.
	 * @param AmountToConsume The amount to consume (default: 1).
	 * @return The remaining quantity after consumption. If the item is not consumable, returns 0.
	 */
	UFUNCTION(BlueprintCallable)
	int32 Consume(const int32 AmountToConsume = 1);

	/**
	 * Returns the full name of the item, including its type and any additional information.
	 * @return The full name of the item.
	 */
	UFUNCTION(BlueprintPure)
	virtual FText GetItemFullName() const;

	/**
	 * Returns the icon of the item, typically used for UI representation.
	 * @return The icon brush of the item.
	 */
	UFUNCTION(BlueprintPure)
	virtual FSlateBrush GetItemIcon() const;

	/**
	 * Returns the description of the item, providing details about its use or lore.
	 * @return The description text of the item.
	 */
	UFUNCTION(BlueprintPure)
	virtual FText GetItemDescription() const;

	/**
	 * Loads the item mesh synchronously.
	 * @return The loaded mesh.
	 */
	UFUNCTION(BlueprintPure)
	UStaticMesh* LoadItemMeshSync() const;

	/**
	 * Loads the item mesh asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadItemMeshAsync() const;

	/**
	 * Returns the weight of a single item instance.
	 * @return The weight of the item.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetItemWeight() const;

	/**
	 * Returns the total weight of the item stack (quantity * single item weight).
	 * @return The total weight of the item stack.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetItemFullWeight() const;

	/**
	 * Returns the equip slot index for this item.
	 * @return The equip slot index.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetEquipSlotIndex() const;

	/**
	 * Returns the equip slot key for this item.
	 * @return Pointer to the UEquipSlotKey.
	 */
	UFUNCTION(BlueprintPure)
	UEquipSlotKey* GetEquipSlotKey() const;

	/**
	 * Checks if the item can be stacked.
	 * @return True if the item can be stacked, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool CanStack() const;

	/**
	 * Checks if this item can be stacked with another item.
	 * @param OtherItem The other item to check for stacking compatibility.
	 * @return True if the items can be stacked together, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanStackItem(UItemBase* OtherItem) const;

	/**
	 * Checks if the item is equippable.
	 * @return True if the item is equippable, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEquippable();

	/**
	 * Checks if the item has permission to be equipped.
	 * @return True if the item can be equipped, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool HasEquipPermission() const;

	/**
	 * Checks if all preconditions for equipping the item are met.
	 * @return True if all preconditions are met, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool AreEquipPreconditionsMet();

	/**
	 * Checks if the item stack is at its maximum allowed quantity.
	 * @return True if the item is max stacked, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsMaxStacked() const;

	/**
	 * Checks if the item is stackable.
	 * @return True if the item is stackable, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsStackable() const;

	/**
	 * Returns the current quantity of the item in the stack.
	 * @return The current quantity.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetCurrentQuantity() const;

	/**
	 * Attempts to stack another item into this one.
	 * @param Item The item to stack.
	 * @param AmountToStack The amount to stack (default: 1).
	 * @return True if stacking was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryStackItem(UItemBase* Item, const int32 AmountToStack = 1);

	/**
	 * Attempts to split the item stack into a new item.
	 * @param AmountToSplit The amount to split from the stack.
	 * @param OutNewSplittedItem Reference to the new splitted item created.
	 * @return True if the split was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TrySplitItem(const int32 AmountToSplit, UItemBase*& OutNewSplittedItem);

	/**
	 * Increases the quantity of the item stack.
	 * @param Amount The amount to increase (default: 1).
	 * @return The new quantity after the increase.
	 */
	int32 IncreaseQuantity(int32 Amount = 1);

	/**
	 * Decreases the quantity of the item stack.
	 * @param Amount The amount to decrease (default: 1).
	 * @return The new quantity after the decrease.
	 */
	int32 DecreaseQuantity(const int32 Amount = 1);

	/**
	 * Sets the quantity of the item stack.
	 * @param Quantity The new quantity to set.
	 * @return The new quantity after the set.
	 */
	int32 SetQuantity(const int32 Quantity);

	UItemBase* DuplicateItem() const;

protected:
	/**
	 * Called before the item is initialized.
	 * @param Data The item data to initialize with.
	 */
	virtual void OnPreInit(UItemDataBase* Data);

	/**
	 * Called when the item is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * Called when the item is added to an inventory.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnAdd();

	/**
	 * Called when the item is removed from an inventory.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnRemove();

	/**
	 * Called when the item is equipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEquip();

	/**
	 * Called when the item is unequipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip();

	/**
	 * Called when the item is dropped into the world.
	 * @param ItemDropActor The actor representing the dropped item.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDrop(AItemDropActor* ItemDropActor);

	/**
	 * Called when the item drop is notified (e.g., for UI feedback).
	 * @param ItemDropActor The actor representing the dropped item.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDropNotify(AItemDropActor* ItemDropActor);

	/**
	 * Called when the item is used.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnUse();

	/**
	 * Called when the item is consumed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnConsume();

	/**
	 * Returns the world associated with this item.
	 * @return Pointer to the UWorld.
	 */
	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	/**
	 * Checks if this object implements GetWorld in the editor.
	 * @return True if GetWorld is implemented, false otherwise.
	 */
	virtual bool ImplementsGetWorld() const override;
#endif

	/**
	 * Sets the mesh to be loaded for this item.
	 */
	virtual void SetItemMeshToLoad();

private:
	/**
	 * Checks if the item can be dropped.
	 * @return True if the item can be dropped, false otherwise.
	 */
	bool CanDrop() const;

	/**
	 * Callback for when the item mesh is loaded asynchronously.
	 */
	void OnAsyncLoadItemMesh() const;
};
