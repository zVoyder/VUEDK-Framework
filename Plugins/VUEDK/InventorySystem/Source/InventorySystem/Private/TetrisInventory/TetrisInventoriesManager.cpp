// Copyright VUEDK, Inc. All Rights Reserved.

#include "TetrisInventory/TetrisInventoriesManager.h"
#include "TetrisInventory/TetrisItem.h"

UTetrisInventoriesManager::UTetrisInventoriesManager()
{
}

bool UTetrisInventoriesManager::TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory)
{
	if (!IsValid(Item) || !IsValid(DestinationInventory))
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisInventoriesManager::TryTransferItem: Params not valid."));
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

bool UTetrisInventoriesManager::TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition)
{
	if (!IsValid(Item) || !IsValid(DestinationInventory))
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisInventoriesManager::TryTransferItemAtPosition: Params not valid."));
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

bool UTetrisInventoriesManager::TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB)
{
	if (!IsValid(ItemA) || !IsValid(ItemB))
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisInventoriesManager::TryTransferSwitchItems: Items not valid."));
		return false;
	}

	if (ItemA->IsEquipped())
	{
		UEquipment* Equipment = ItemA->RelatedEquipment;

		if (!IsValid(Equipment))
		{
			UE_LOG(LogTemp, Error, TEXT("UTetrisInventoriesManager::TryTransferSwitchItems: Equipment is not valid."));
			return false;
		}

		return Equipment->TryEquipItem(ItemB, ItemA->GetItemData()->EquipSlotKey, ItemA->GetEquipSlotIndex());
	}
	
	UTetrisInventory* SourceInventory = Cast<UTetrisInventory>(ItemA->RelatedInventory);
	UTetrisInventory* DestinationInventory = Cast<UTetrisInventory>(ItemB->RelatedInventory);

	if (!IsValid(SourceInventory) || !IsValid(DestinationInventory))
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisInventoriesManager::TryTransferSwitchItems: Source or Destination inventory is not valid."));
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
