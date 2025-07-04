// Copyright VUEDK, Inc. All Rights Reserved.

#include "EquipmentSystem/Equipment.h"
#include "Base/InventoryBase.h"
#include "Base/Data/ItemDataBase.h"

UEquipment::UEquipment()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEquipment::TryEquipItemToAvailableSlot(UItemBase* Item, const int32 SwapPriority, const bool bAllowSwapWithInventoryItem, const bool bNotifyEvent)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()))
		return false;

	UEquipSlotKey* ItemSlotKey = Item->GetItemData()->EquipSlotKey;
	if (!IsValid(ItemSlotKey) || !EquipSlots.Contains(ItemSlotKey->EquipSlotKey))
		return false;

	if (AreAllSlotsOfKeyOccupied(ItemSlotKey))
	{
		if (TryEquipItem(Item, ItemSlotKey, SwapPriority, bAllowSwapWithInventoryItem, bNotifyEvent))
			return true;
	}

	auto FindAndTryEquip = [&](const bool bAllowSwap)
	{
		for (int32 i = 0; i < SlotLimits[ItemSlotKey]; i++)
		{
			if (TryEquipItem(Item, ItemSlotKey, i, bAllowSwap, bNotifyEvent))
				return true;
		}

		return false;
	};

	return FindAndTryEquip(false) || (bAllowSwapWithInventoryItem && FindAndTryEquip(true));
}

bool UEquipment::TryEquipItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, const int32 SlotIndex, const bool bAllowSwapWithInventoryItem, const bool bNotifyEvent)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()) || !IsValid(TargetSlotKey))
	{
		UE_LOG(LogEquipmentSystem, Warning, TEXT("UEquipment::TryEquipItem: Item or TargetSlotKey is not valid."));
		return false;
	}

	if (!Item->IsEquippable())
	{
		UE_LOG(LogEquipmentSystem, Display, TEXT("UEquipment::TryEquipItem: Item %s is currently not equippable."), *Item->GetItemData()->ItemTypeID);
		return false;
	}

	UEquipSlotKey* EquipSlotKey = Item->GetItemData()->EquipSlotKey;
	if (!IsSlotKeyValid(Item, EquipSlotKey, SlotIndex) || !HasSameSlotKey(Item, TargetSlotKey))
	{
		UE_LOG(LogEquipmentSystem, Warning, TEXT("UEquipment::TryEquipItem: Item slot key is not valid."));
		return false;
	}

	if (!IsSlotKeyEmpty(Item, EquipSlotKey, SlotIndex))
	{
		UItemBase* EquippedItem = EquipSlots[EquipSlotKey->EquipSlotKey][SlotIndex];

		// If the item is going to change slot with an other empty one
		if (Item->IsEquipped())
			return TrySwitchEquippedItems(EquippedItem, Item);

		if (!bAllowSwapWithInventoryItem)
			return false;

		// If the item is going to swap with an other item in the inventory
		return TrySwapWithInventoryItem(Item, EquipSlotKey, SlotIndex, EquippedItem);
	}

	if (Item->IsEquipped())
	{
		// If the item is already equipped, we need to change its slot
		ChangeItemEquipSlot(Item, EquipSlotKey, SlotIndex);
		return true;
	}

	EquipItem(Item, EquipSlotKey, SlotIndex, true, bNotifyEvent);
	return true;
}

bool UEquipment::TryUnequipItem(UItemBase* Item, const bool bNotifyEvent)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()))
		return false;

	UEquipSlotKey* EquipSlotKey = Item->GetItemData()->EquipSlotKey;

	if (!IsValid(EquipSlotKey) || !EquipSlots.Contains(EquipSlotKey->EquipSlotKey))
		return false;

	const TMap<int32, UItemBase*> SlotItems = EquipSlots[EquipSlotKey->EquipSlotKey];
	const int32* SlotIndexRef = SlotItems.FindKey(Item);

	if (SlotIndexRef == nullptr)
		return false;

	UnequipItem(Item, EquipSlotKey, *SlotIndexRef, bNotifyEvent);
	return true;
}

TSet<UItemBase*> UEquipment::GetEquippedItems()
{
	TSet<UItemBase*> Items;
	for (const auto& SlotItems : EquipSlots)
	{
		for (const auto& Item : SlotItems.Value)
		{
			if (Item.Value != nullptr)
				Items.Add(Item.Value);
		}
	}

	return Items;
}

void UEquipment::ClearEquipment()
{
	for (auto& SlotItems : EquipSlots)
	{
		for (auto& Item : SlotItems.Value)
			Item.Value = nullptr;
	}

	OnEquipmentCleared.Broadcast();
	OnEquipChanged.Broadcast();
}

bool UEquipment::AreAllSlotsOfKeyOccupied(const UEquipSlotKey* TargetSlotKey) const
{
	if (!IsValid(TargetSlotKey) || !EquipSlots.Contains(TargetSlotKey->EquipSlotKey))
		return false;

	const TMap<int32, UItemBase*> SlotItems = EquipSlots[TargetSlotKey->EquipSlotKey];

	for (const auto& Slot : SlotItems)
	{
		if (!IsValid(Slot.Value))
			return false;
	}

	return true;
}

UItemBase* UEquipment::FindEquippedItem(const UEquipSlotKey* TargetSlotKey) const
{
	if (!IsValid(TargetSlotKey) || !EquipSlots.Contains(TargetSlotKey->EquipSlotKey))
		return nullptr;

	const TMap<int32, UItemBase*> SlotItems = EquipSlots[TargetSlotKey->EquipSlotKey];
	for (const auto& Slot : SlotItems)
	{
		if (Slot.Value != nullptr)
			return Slot.Value;
	}

	return nullptr;
}

UItemBase* UEquipment::FindEquippedItemAtIndex(const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const
{
	if (!IsValid(TargetSlotKey) || !EquipSlots.Contains(TargetSlotKey->EquipSlotKey))
		return nullptr;

	const TMap<int32, UItemBase*> SlotItems = EquipSlots[TargetSlotKey->EquipSlotKey];
	if (SlotItems.Contains(SlotIndex))
		return SlotItems[SlotIndex];

	return nullptr;
}

void UEquipment::BeginPlay()
{
	Super::BeginPlay();

	EquipSlots = TMap<FName, TMap<int32, UItemBase*>>();
	for (const auto& SlotLimit : SlotLimits)
	{
		if (SlotLimit.Key == nullptr)
			continue;

		TMap<int32, UItemBase*> IndexedItems = TMap<int32, UItemBase*>();

		for (int32 i = 0; i < SlotLimit.Value; i++)
			IndexedItems.Add(i, nullptr);

		EquipSlots.Add(SlotLimit.Key->EquipSlotKey, IndexedItems);
	}
}

bool UEquipment::IsSlotKeyValid(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const
{
	if (!IsValid(TargetSlotKey) || !EquipSlots.Contains(TargetSlotKey->EquipSlotKey))
		return false;

	TMap<int32, UItemBase*> SlotItems = EquipSlots[TargetSlotKey->EquipSlotKey];
	return SlotIndex >= 0 && SlotIndex < SlotLimits[TargetSlotKey];
}

bool UEquipment::HasSameSlotKey(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey)
{
	if (!IsValid(TargetSlotKey) || !IsValid(Item->GetItemData()->EquipSlotKey))
		return false;

	return Item->GetItemData()->EquipSlotKey == TargetSlotKey;
}

bool UEquipment::IsSlotKeyEmpty(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const
{
	if (!EquipSlots.Contains(TargetSlotKey->EquipSlotKey))
		return false;

	TMap<int32, UItemBase*> SlotItems = EquipSlots[TargetSlotKey->EquipSlotKey];
	return SlotItems.Contains(SlotIndex) && SlotItems[SlotIndex] == nullptr;
}

void UEquipment::OnEquipItem_Implementation(UEquipSlotKey* EquipSlotKey, UItemBase* Item)
{
}

void UEquipment::OnUnequipItem_Implementation(UEquipSlotKey* EquipSlotKey, UItemBase* Item)
{
}

void UEquipment::EquipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory, const bool bNotifyEvent)
{
	AddItemInEquipSlot(Item, EquipSlotKey, SlotIndex);

	if (bRemoveFromInventory)
		Item->Remove();

	// Unique dynamic binding because the item can reinsert itself in the inventory slot
	Item->OnItemAdded.AddUniqueDynamic(this, &UEquipment::OnItemAddedToAnyInventory); // If the item is added to any inventory or equipped to another slot, remove it from the current slot

	if (bNotifyEvent)
		OnAnyItemEquipNotify.Broadcast(EquipSlotKey, SlotIndex, Item);

	OnAnyItemEquipped.Broadcast(EquipSlotKey, SlotIndex, Item);
	OnEquipChanged.Broadcast();
}

void UEquipment::UnequipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bNotifyEvent)
{
	RemoveItemFromEquipSlot(Item, EquipSlotKey, SlotIndex);
	Item->OnItemAdded.RemoveDynamic(this, &UEquipment::OnItemAddedToAnyInventory);

	if (bNotifyEvent)
		OnAnyItemUnequipNotify.Broadcast(EquipSlotKey, SlotIndex, Item);

	OnAnyItemUnequipped.Broadcast(EquipSlotKey, SlotIndex, Item);
	OnEquipChanged.Broadcast();
}

void UEquipment::AddItemInEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex)
{
	EquipSlots[EquipSlotKey->EquipSlotKey][NewSlotIndex] = Item;
	Item->SetEquipSlot(this, NewSlotIndex);
}

void UEquipment::RemoveItemFromEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex)
{
	EquipSlots[EquipSlotKey->EquipSlotKey][SlotIndex] = nullptr;
	Item->ClearEquipSlot();
}

void UEquipment::ChangeItemEquipSlot(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex)
{
	const TMap<int32, UItemBase*> SlotItems = EquipSlots[EquipSlotKey->EquipSlotKey];
	const int32* OldSlotIndexRef = SlotItems.FindKey(Item);

	if (*OldSlotIndexRef == NewSlotIndex) // If the item is already in the desired slot, do nothing
		return;

	if (OldSlotIndexRef == nullptr) // Safe check to prevent crashes
	{
		EquipItem(Item, EquipSlotKey, NewSlotIndex, true, false);
		return;
	}

	UnequipItem(Item, EquipSlotKey, *OldSlotIndexRef, false);
	EquipItem(Item, EquipSlotKey, NewSlotIndex, true, false);
	OnAnyItemEquipSlotChanged.Broadcast(EquipSlotKey, Item, NewSlotIndex, *OldSlotIndexRef);
	OnEquipChanged.Broadcast();
}

bool UEquipment::TrySwitchEquippedItems(UItemBase* ItemA, UItemBase* ItemB)
{
	UEquipSlotKey* EquipSlotKey = ItemA->GetItemData()->EquipSlotKey;

	if (EquipSlotKey != ItemB->GetItemData()->EquipSlotKey)
		return false;

	const TMap<int32, UItemBase*> SlotItems = EquipSlots[EquipSlotKey->EquipSlotKey];
	const int32 ItemAIndex = *SlotItems.FindKey(ItemA);
	const int32 ItemBIndex = *SlotItems.FindKey(ItemB);

	if (TryUnequipItem(ItemA, false) && TryUnequipItem(ItemB, false))
	{
		EquipItem(ItemA, EquipSlotKey, ItemBIndex, false, false);
		EquipItem(ItemB, EquipSlotKey, ItemAIndex, false, false);
	}
	else
	{
		// If the unequip fails, we need to re-equip the items to their original slots
		EquipItem(ItemA, EquipSlotKey, ItemAIndex, false, false);
		EquipItem(ItemB, EquipSlotKey, ItemBIndex, false, false);
		return false;
	}

	OnItemsSwitchedSlots.Broadcast(EquipSlotKey, ItemA, ItemB, ItemAIndex, ItemBIndex, ItemBIndex, ItemAIndex);
	OnEquipChanged.Broadcast();
	return true;
}

bool UEquipment::TrySwapWithInventoryItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, int32 SlotIndex, UItemBase* EquippedItem)
{
	UInventoryBase* Inventory = Item->RelatedInventory;

	if (!IsValid(Inventory))
		return false;

	Item->Remove(); // Remove it to make space
	EquippedItem->OnItemAdded.RemoveDynamic(this, &UEquipment::OnItemAddedToAnyInventory); // Unbind it to prevent the UnequipItem from being called again

	if (Inventory->TryAddItem(EquippedItem))
	{
		UnequipItem(EquippedItem, TargetSlotKey, SlotIndex, false);
		EquipItem(Item, TargetSlotKey, SlotIndex, false, false);
		OnAnyItemSwapWithInventoryItem.Broadcast(TargetSlotKey, Item, EquippedItem, SlotIndex);
		OnEquipChanged.Broadcast();
		return true;
	}

	// If it fails, add the item back to the inventory
	Inventory->TryAddItem(Item);
	return false;
}

void UEquipment::OnItemAddedToAnyInventory(UItemBase* Item, UInventoryBase* Inventory)
{
	TryUnequipItem(Item);
}
