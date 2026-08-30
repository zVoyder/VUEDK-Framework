// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/CharacterStats/CharacterStats.h"
#include "RPGSystem.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGInventory/Items/RPGItem.h"

UCharacterStats::UCharacterStats(): Equipment(nullptr)
{
}

void UCharacterStats::Init(UEquipment* InEquipment)
{
	if (!IsValid(InEquipment))
		return;
	
	Equipment = InEquipment;
	Equipment->OnAnyItemEquipNotify.AddDynamic(this, &UCharacterStats::OnItemEquippedOrUnequipped);
	Equipment->OnAnyItemUnequipNotify.AddDynamic(this, &UCharacterStats::OnItemEquippedOrUnequipped);
	Equipment->OnAnyItemSwapWithInventoryItem.AddDynamic(this, &UCharacterStats::OnItemSwappedWithInventoryItem);
	Equipment->OnItemsSwitchedSlots.AddDynamic(this, &UCharacterStats::OnItemsSwitchedSlots);
}

void UCharacterStats::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!Check())
		return;
	
	Equipment->OnAnyItemEquipNotify.RemoveDynamic(this, &UCharacterStats::OnItemEquippedOrUnequipped);
	Equipment->OnAnyItemUnequipNotify.RemoveDynamic(this, &UCharacterStats::OnItemEquippedOrUnequipped);
	Equipment->OnAnyItemSwapWithInventoryItem.RemoveDynamic(this, &UCharacterStats::OnItemSwappedWithInventoryItem);
	Equipment->OnItemsSwitchedSlots.RemoveDynamic(this, &UCharacterStats::OnItemsSwitchedSlots);
}

void UCharacterStats::NativeOnCalculateFullStatsValues()
{
	Super::NativeOnCalculateFullStatsValues();
	AddEquippedItemsStats();
}

void UCharacterStats::OnItemEquippedOrUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* ItemBase)
{
	CalculateFullStatsValues();
}

void UCharacterStats::OnItemSwappedWithInventoryItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item, UItemBase* InventoryItem, int32 SlotIndex)
{
	CalculateFullStatsValues();
}

void UCharacterStats::OnItemsSwitchedSlots(UEquipSlotKey* EquipSlotKey, UItemBase* ItemBaseA, UItemBase* ItemBaseB, int OldSlotIndexItemA, int OldSlotIndexItemB, int NewSlotIndexItemA, int NewSlotIndexItemB)
{
	CalculateFullStatsValues();
}

void UCharacterStats::AddEquippedItemsStats() const
{
	if (!Check())
		return;
	
	for (UItemBase* Item : Equipment->GetEquippedItems())
	{
		const URPGGearItem* RPGGearItem = Cast<URPGGearItem>(Item);
		if (!IsValid(RPGGearItem))
		{
			UE_LOG(LogStatsSystem, Warning, TEXT("UCharacterStats::OnCalculateFullStatsValues_Implementation: Item %s is not a valid RPGGearItem."), *Item->GetFullName());
			continue;
		}

		const URPGGearItemStatsContainer* GearStatsContainer = RPGGearItem->GearStatsContainer;
		if (!IsValid(GearStatsContainer))
		{
			UE_LOG(LogStatsSystem, Warning, TEXT("UCharacterStats::OnCalculateFullStatsValues_Implementation: Item %s has no valid GearStatsContainer."), *Item->GetFullName());
			continue;
		}
		
		for (const auto ItemStats : GearStatsContainer->GetValues())
			ModifyFullStatValue(ItemStats.Key, ItemStats.Value);
	}
}

bool UCharacterStats::Check() const
{
	return IsValid(Equipment);
}
