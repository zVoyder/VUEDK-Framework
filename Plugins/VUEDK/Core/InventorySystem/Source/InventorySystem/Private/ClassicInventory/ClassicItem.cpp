// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassicInventory/ClassicItem.h"
#include "ClassicInventory/Data/ClassicItemData.h"
#include "ClassicInventory/Data/ClassicItemSlotData.h"

// FClassicItemSaveData UClassicItem::CreateClassicItemSaveData() const
// {
// 	FClassicItemSaveData ClassicItemSaveData;
// 	ClassicItemSaveData.ItemSaveData = CreateItemBaseSaveData();
// 	return ClassicItemSaveData;
// }
//
// void UClassicItem::LoadClassicItemSaveData(UInventoryBase* LoadingInventory, const FClassicItemSaveData ItemSaveData)
// {
// 	bool bHasBeenEquipped;
// 	LoadItemBaseSaveData(LoadingInventory, ItemSaveData.ItemSaveData, bHasBeenEquipped);
// }

UClassicItemData* UClassicItem::GetClassicItemData() const
{
	return Cast<UClassicItemData>(ItemData);
}

TArray<FClassicItemSlotData> UClassicItem::GetRequiredSlots() const
{
	const int32 ItemMaxStack = ItemData->MaxStackSize;
	const int32 NeededSlots = GetMinRequiredSlots();

	TArray<FClassicItemSlotData> Slots;
	for (int32 i = 0; i < NeededSlots; i++)
	{
		FClassicItemSlotData Slot;
		Slot.StackSize = ItemMaxStack;
		Slot.SlotQuantity = FMath::Min(GetCurrentQuantity() - (i * ItemMaxStack), ItemMaxStack);
		Slots.Add(Slot);
	}

	return Slots;
}

int32 UClassicItem::GetMinRequiredSlots() const
{
	const int32 ItemMaxStack = ItemData->MaxStackSize;
	int32 NeededSlotsCount = GetCurrentQuantity() / ItemMaxStack;
	if (GetCurrentQuantity() % ItemMaxStack > 0) NeededSlotsCount++;

	return NeededSlotsCount;
}
