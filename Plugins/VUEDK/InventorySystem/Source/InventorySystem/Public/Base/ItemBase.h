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

	virtual void Init(UObject* WorldContextObject, UItemDataBase* Data);

	UFUNCTION(BlueprintPure)
	UItemDataBase* GetItemData() const;

	void AssignInventory(UInventoryBase* Inventory);

	void DeassignInventory();

	void SetEquipSlot(UEquipment* InEquipment, const int32 SlotIndex);

	void ClearEquipSlot();

	UFUNCTION(BlueprintPure)
	bool IsEquipped() const;

	UFUNCTION(BlueprintCallable)
	bool TryDrop(const FVector Location, const FRotator Rotation, AItemDropActor*& OutItemDropActor, const bool bNotify = true);

	UFUNCTION(BlueprintCallable)
	void Remove();

	UFUNCTION(BlueprintCallable)
	void Use();

	UFUNCTION(BlueprintCallable)
	int32 Consume(const int32 AmountToConsume = 1);

	UFUNCTION(BlueprintPure)
	virtual FText GetItemFullName() const;

	UFUNCTION(BlueprintPure)
	virtual FSlateBrush GetItemIcon() const;

	UFUNCTION(BlueprintPure)
	virtual FText GetItemDescription() const;

	/**
	 * @brief Loads the item mesh synchronously.
	 * @return The loaded mesh.
	 */
	UFUNCTION(BlueprintPure)
	UStaticMesh* LoadItemMeshSync() const;

	/**
	 * @brief Loads the item mesh asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadItemMeshAsync() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetItemWeight() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetItemFullWeight() const;

	UFUNCTION(BlueprintPure)
	int32 GetEquipSlotIndex() const;

	UFUNCTION(BlueprintPure)
	UEquipSlotKey* GetEquipSlotKey() const;

	UFUNCTION(BlueprintPure)
	bool CanStack() const;

	UFUNCTION(BlueprintNativeEvent)
	bool CanStackItem(UItemBase* OtherItem) const;

	UFUNCTION(BlueprintPure)
	bool IsEquippable();

	UFUNCTION(BlueprintNativeEvent)
	bool HasEquipPermission() const;

	UFUNCTION(BlueprintPure)
	bool AreEquipPreconditionsMet();

	UFUNCTION(BlueprintPure)
	bool IsMaxStacked() const;

	UFUNCTION(BlueprintPure)
	bool IsStackable() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentQuantity() const;

	UFUNCTION(BlueprintCallable)
	bool TryStackItem(UItemBase* Item, const int32 AmountToStack = 1);

	UFUNCTION(BlueprintCallable)
	bool TrySplitItem(const int32 AmountToSplit, UItemBase*& OutNewSplittedItem);

	int32 IncreaseQuantity(int32 Amount = 1);

	int32 DecreaseQuantity(const int32 Amount = 1);

	int32 SetQuantity(const int32 Quantity);

	UItemBase* DuplicateItem() const;

protected:
	virtual void OnPreInit(UItemDataBase* Data);

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnAdd();

	UFUNCTION(BlueprintNativeEvent)
	void OnRemove();

	UFUNCTION(BlueprintNativeEvent)
	void OnEquip();

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip();

	UFUNCTION(BlueprintNativeEvent)
	void OnDrop(AItemDropActor* ItemDropActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnDropNotify(AItemDropActor* ItemDropActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnUse();

	UFUNCTION(BlueprintNativeEvent)
	void OnConsume();

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual void SetItemMeshToLoad();

private:
	bool CanDrop() const;

	void OnAsyncLoadItemMesh() const;
};
