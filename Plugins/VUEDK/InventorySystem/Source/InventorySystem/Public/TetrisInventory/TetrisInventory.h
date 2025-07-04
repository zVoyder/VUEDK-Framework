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

	UFUNCTION(BlueprintPure)
	UTetrisSlot* GetSlot(const FIntPoint SlotPosition) const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetGridSize() const;

	UFUNCTION(BlueprintPure)
	bool CanOccupySlots(const FIntPoint Size, const FIntPoint StartSlotPosition, bool& bOutNeedsRotation, const bool bCheckRotation = false, const UTetrisItem* IgnoreItem = nullptr) const;

	UFUNCTION(BlueprintCallable)
	bool TryAddNewItemAtSlots(UTetrisItemData* ItemData, const FIntPoint StartSlotPosition);

	UFUNCTION(BlueprintCallable)
	bool TryAddItemAtSlots(UTetrisItem* Item, const FIntPoint StartSlotPosition);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "bCallEvents"))
	bool TryMoveItem(UTetrisItem* Item, const FIntPoint NewPosition, const bool bTryRotate = false, const UTetrisItem* IgnoreItem = nullptr);

	UFUNCTION(BlueprintCallable)
	bool TrySwitchItemsPositions(UTetrisItem* ItemA, UTetrisItem* ItemB);

protected:
	virtual void BeginPlay() override;

	virtual USaveData* CreateSaveDataObject_Implementation() override;

	virtual USaveData* CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave) override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	virtual void OnItemAdded_Implementation(UItemBase* Item) override;

	virtual void OnItemRemoved_Implementation(UItemBase* Item) override;

	virtual void OnClearedInventory_Implementation() override;

private:
	void ConstructGrid();

	bool IsValidSlotPosition(const FIntPoint SlotPosition) const;

	void FreeSlots(const FIntPoint StartPosition, const FIntPoint Size);

	void OccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList = true);

	bool TryOccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bTryRotate = false, const UTetrisItem* IgnoreItem = nullptr, const bool bAddItemToList = true);

	bool FindAvailableSlots(const FIntPoint Size, FIntPoint& OutStartSlotPosition, bool& OutbNeedsRotation) const;
};
