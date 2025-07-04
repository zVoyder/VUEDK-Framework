// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSystem/Equipment.h"
#include "StatsSystem/StatsBridgeBase.h"
#include "CharacterStats.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGSYSTEM_API UCharacterStats : public UStatsBridgeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UEquipment* Equipment;
	
public:
	UCharacterStats();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/**
	 * Initializes the character stats with the provided equipment.
	 * @param InEquipment - The equipment to associate with these stats.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment);

protected:
	/**
	 * Called to calculate the full stats values for the character.
	 * Override from UStatsBridgeBase.
	 */
	virtual void NativeOnCalculateFullStatsValues() override;
	
private:
	/**
	 * Called when an item is equipped or unequipped.
	 * @param EquipSlotKey - The equipment slot key affected.
	 * @param SlotIndex - The index of the slot.
	 * @param ItemBase - The item being equipped or unequipped.
	 */
	UFUNCTION()
	void OnItemEquippedOrUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* ItemBase);

	/**
	 * Called when an item is swapped with an inventory item.
	 * @param EquipSlotKey - The equipment slot key affected.
	 * @param Item - The item being equipped.
	 * @param InventoryItem - The item from the inventory.
	 * @param SlotIndex - The index of the slot.
	 */
	UFUNCTION()
	void OnItemSwappedWithInventoryItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item, UItemBase* InventoryItem, int32 SlotIndex);
	
	/**
	 * Called when two items are switched between slots.
	 * @param EquipSlotKey - The equipment slot key affected.
	 * @param ItemBaseA - The first item being switched.
	 * @param ItemBaseB - The second item being switched.
	 * @param OldSlotIndexItemA - The old slot index of item A.
	 * @param OldSlotIndexItemB - The old slot index of item B.
	 * @param NewSlotIndexItemA - The new slot index of item A.
	 * @param NewSlotIndexItemB - The new slot index of item B.
	 */
	UFUNCTION()
	void OnItemsSwitchedSlots(UEquipSlotKey* EquipSlotKey, UItemBase* ItemBaseA, UItemBase* ItemBaseB, int32 OldSlotIndexItemA, int32 OldSlotIndexItemB, int32 NewSlotIndexItemA, int32 NewSlotIndexItemB);
	
	/**
	 * Adds the stats of all equipped items to the character's stats.
	 */
	void AddEquippedItemsStats() const;
	
	/**
	 * Checks if the character stats are valid and ready for calculation.
	 * @return true if valid, false otherwise.
	 */
	bool Check() const;
};
