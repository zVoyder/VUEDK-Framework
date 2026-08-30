// Copyright VUEDK, Inc. All Rights Reserved.

#include "ClassicInventory/ClassicInventory.h"
#include "InventorySystem.h"
#include "ClassicInventory/ClassicItem.h"
#include "ClassicInventory/Data/ClassicItemSlotData.h"
#include "Utility/ISInventoriesUtility.h"

class UClassicInventorySaveData;

UClassicInventory::UClassicInventory()
{
}

void UClassicInventory::OnItemAdded_Implementation(UItemBase* Item)
{
	if (UItemBase* FoundItem = Find(Item->GetItemData()))
	{
		UClassicItem* ClassicItem = Cast<UClassicItem>(FoundItem);
		ClassicItem->IncreaseQuantity(); // Increase quantity of the item, if it already exists in the inventory
	}
}

int32 UClassicInventory::GetTotalMinRequiredSlotsCount() const
{
	int32 CurrSlots = 0;
	
	for (const UItemBase* Item : Items)
	{
		const UClassicItem* ClassicItem = Cast<UClassicItem>(Item);
		CurrSlots += ClassicItem->GetMinRequiredSlots();
	}
	
	return CurrSlots;
}

bool UClassicInventory::IsFull() const
{
	if (IsSatisfyingAllRequiredSlots()) return false;

	for (const UItemBase* Item : Items)
	{
		const UClassicItem* ClassicItem = Cast<UClassicItem>(Item);
		for (const FClassicItemSlotData& Slot : ClassicItem->GetRequiredSlots()) // For each slot the item occupies
		{
			if (!Slot.IsMaxStacked())
				return false;
		}
	}

	return true;
}

bool UClassicInventory::IsSatisfyingAllRequiredSlots() const
{
	return GetTotalMinRequiredSlotsCount() < Capacity;
}

bool UClassicInventory::CanContainItem(const UItemDataBase* ItemData) const
{
	if (!UISInventoriesUtility::IsItemInRegistry(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is not registered in the inventory registry %s."), *ItemData->ItemTypeId, *GetName());
		return false;
	}
	
	if (IsFull())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, inventory %s is full."), *GetName());
		return false;
	}

	if (const UItemBase* FoundItem = Find(ItemData))
	{
		if (FoundItem->GetItemData()->bIsUnique)
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is unique and already exists in the inventory %s."), *ItemData->ItemTypeId, *GetName());
			return false;
		}

		const UClassicItem* ClassicItem = Cast<UClassicItem>(FoundItem);
		for (const FClassicItemSlotData& Slot : ClassicItem->GetRequiredSlots())
		{
			if (!Slot.IsMaxStacked())
				return true;
		}
	}

	return IsSatisfyingAllRequiredSlots();
}
