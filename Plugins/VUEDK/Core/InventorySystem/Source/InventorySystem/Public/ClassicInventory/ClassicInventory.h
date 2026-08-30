// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Base/InventoryBase.h"
#include "ClassicInventory.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UClassicInventory : public UInventoryBase
{
	GENERATED_BODY()

public:
	UClassicInventory();
	
	virtual void OnItemAdded_Implementation(UItemBase* Item) override;

	/**
	 * Returns the total number of slots required by all items in the inventory.
	 * @return The total number of slots required.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetTotalMinRequiredSlotsCount() const;

	/**
	 * Checks if the inventory is full.
	 * @return True if the inventory is full, false otherwise.
	 */
	virtual bool IsFull() const override;

	/**
	 * Checks if the inventory is satisfying all required slots for the items it contains.
	 * @return True if all required slots are satisfied, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsSatisfyingAllRequiredSlots() const;

	/**
	 * Checks if the inventory can contain the specified item.
	 * @param ItemData The item data to check.
	 * @return True if the inventory can contain the item, false otherwise.
	 */
	virtual bool CanContainItem(const UItemDataBase* ItemData) const override;
};
