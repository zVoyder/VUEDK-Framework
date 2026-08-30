// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/TetrisInventoriesUtility.h"
#include "InventorySystem.h"
#include "Utility/ISInventoriesUtility.h"

TWeakObjectPtr<UTetrisInventoryEventsHandlerSubsystem> UTetrisInventoriesUtility::EventsHandlerSubsystem = nullptr;

void UTetrisInventoriesUtility::SetTetrisEventsHandler(UTetrisInventoryEventsHandlerSubsystem* InEventsHandlerSubsystem)
{
	EventsHandlerSubsystem = InEventsHandlerSubsystem;
}

bool UTetrisInventoriesUtility::TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory)
{
	if (!IsValid(Item) || !IsValid(DestinationInventory))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventoriesManager::TryTransferItem: Params not valid."));
		return false;
	}

	UTetrisInventory* SourceInventory = Cast<UTetrisInventory>(Item->RelatedInventory);
	if (SourceInventory == DestinationInventory)
		return false;

	const FIntPoint CachedPosition = Item->GetCurrentPosition();
	Item->Remove();
	
	if (DestinationInventory->TryAddItem(Item))
		return true;

	SourceInventory->TryAddItemAtSlots(Item, CachedPosition);
	return false;
}

bool UTetrisInventoriesUtility::TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition)
{
	if (!IsValid(Item) || !IsValid(DestinationInventory))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventoriesManager::TryTransferItemAtPosition: Params not valid."));
		return false;
	}

	UTetrisInventory* SourceInventory = Cast<UTetrisInventory>(Item->RelatedInventory);
	
	if (SourceInventory == DestinationInventory)
		return SourceInventory->TryMoveItem(Item, DestinationPosition);

	bool bNeedsRotation = false;
	if (DestinationInventory->CanOccupySlots(Item->GetRelativeSize(), DestinationPosition, bNeedsRotation, false, nullptr))
	{
		Item->Remove();
		DestinationInventory->TryAddItemAtSlots(Item, DestinationPosition);
		return true;
	}

	return false;
}

bool UTetrisInventoriesUtility::TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB)
{
	if (!IsValid(ItemA) || !IsValid(ItemB))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventoriesManager::TryTransferSwitchItems: Items not valid."));
		return false;
	}

	if (ItemA->IsEquipped())
	{
		UEquipment* Equipment = ItemA->RelatedEquipment;

		if (!IsValid(Equipment))
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventoriesManager::TryTransferSwitchItems: Equipment is not valid."));
			return false;
		}

		return Equipment->TryEquipItem(ItemB, ItemA->GetItemData()->EquipSlotKey, ItemA->GetEquipSlotIndex());
	}
	
	UTetrisInventory* SourceInventory = Cast<UTetrisInventory>(ItemA->RelatedInventory);
	UTetrisInventory* DestinationInventory = Cast<UTetrisInventory>(ItemB->RelatedInventory);

	if (!IsValid(SourceInventory) || !IsValid(DestinationInventory))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventoriesManager::TryTransferSwitchItems: Source or Destination inventory is not valid."));
		return false;
	}
	
	if (SourceInventory == DestinationInventory)
		return SourceInventory->TrySwitchItemsPositions(ItemA, ItemB);

	bool bItemANeedsRotation = false;
	bool bItemBNeedsRotation = false;
	if (DestinationInventory->CanOccupySlots(ItemA->GetRelativeSize(), ItemB->GetCurrentPosition(), bItemANeedsRotation, true, ItemB))
	{
		if (SourceInventory->CanOccupySlots(ItemB->GetRelativeSize(), ItemA->GetCurrentPosition(), bItemBNeedsRotation, true, ItemA))
		{
			if (bItemANeedsRotation)
				ItemA->Rotate();

			if (bItemBNeedsRotation)
				ItemB->Rotate();
			
			ItemA->Remove();
			ItemB->Remove();

			DestinationInventory->TryAddItemAtSlots(ItemA, ItemB->GetCurrentPosition());
			SourceInventory->TryAddItemAtSlots(ItemB, ItemA->GetCurrentPosition());
			return true;
		}
	}

	return false;
}

void UTetrisInventoriesUtility::RequestDrag()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestDrag();
}

void UTetrisInventoriesUtility::RequestDrop()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestDrop();
}

void UTetrisInventoriesUtility::RequestRotate()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestRotate();
}

void UTetrisInventoriesUtility::RequestEquipUnequip(const int32 SwapPriority)
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestEquipUnequip(SwapPriority);
}

void UTetrisInventoriesUtility::RequestTransferItemToTargetInventory()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestTransferItemToTargetInventory();
}

void UTetrisInventoriesUtility::RequestDiscard()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestDiscard();
}

void UTetrisInventoriesUtility::RequestSplit()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestSplit();
}

void UTetrisInventoriesUtility::RequestCancelSplit()
{
	if (!Check())
		return;

	EventsHandlerSubsystem.Get()->RequestCancelSplit();
}

bool UTetrisInventoriesUtility::IsSplitting()
{
	if (!Check())
		return false;

	return EventsHandlerSubsystem.Get()->IsSplitting();
}

bool UTetrisInventoriesUtility::Check()
{
	return EventsHandlerSubsystem.IsValid();
}
