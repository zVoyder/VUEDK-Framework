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
	
	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment);

protected:
	virtual void NativeOnCalculateFullStatsValues() override;
	
private:
	UFUNCTION()
	void OnItemEquippedOrUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* ItemBase);

	UFUNCTION()
	void OnItemSwappedWithInventoryItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item, UItemBase* InventoryItem, int32 SlotIndex);
	
	UFUNCTION()
	void OnItemsSwitchedSlots(UEquipSlotKey* EquipSlotKey, UItemBase* ItemBaseA, UItemBase* ItemBaseB, int32 OldSlotIndexItemA, int32 OldSlotIndexItemB, int32 NewSlotIndexItemA, int32 NewSlotIndexItemB);
	
	void AddEquippedItemsStats() const;
	
	bool Check() const;
};
