// Copyright VUEDK, Inc. All Rights Reserved.

#include "TetrisInventory/TetrisInventory.h"
#include "InventorySystem.h"
#include "TetrisInventory/TetrisItem.h"
#include "TetrisInventory/Data/TetrisItemData.h"
#include "Factories/ISFactory.h"

UTetrisInventory::UTetrisInventory(): GridSize()
{
}

bool UTetrisInventory::CanContainItem(const UItemDataBase* ItemData) const
{
	const UTetrisItemData* TetrisItemData = Cast<UTetrisItemData>(ItemData);

	bool bNeedsRotation;
	if (FIntPoint FoundSlot; !FindAvailableSlots(TetrisItemData->Size, FoundSlot, bNeedsRotation))
		return false;

	return Super::CanContainItem(ItemData);
}

bool UTetrisInventory::IsFull() const
{
	if (Super::IsFull()) return true;

	for (int32 x = 0; x < GridSize.X; x++)
	{
		for (int32 y = 0; y < GridSize.Y; y++)
		{
			if (!InvMatrix[x][y]->IsOccupied())
				return false;
		}
	}

	return true;
}

UTetrisSlot* UTetrisInventory::GetSlot(const FIntPoint SlotPosition) const
{
	if (!IsValidSlotPosition(SlotPosition))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventory::GetSlot: Invalid SlotPosition (%d, %d)."), SlotPosition.X, SlotPosition.Y);
		return nullptr;
	}

	return InvMatrix[SlotPosition.X][SlotPosition.Y];
}

FIntPoint UTetrisInventory::GetGridSize() const
{
	return GridSize;
}

bool UTetrisInventory::CanOccupySlots(const FIntPoint Size, const FIntPoint StartSlotPosition, bool& bOutNeedsRotation, const bool bCheckRotation, const UTetrisItem* IgnoreItem) const
{
	auto CheckSlots = [&](const FIntPoint& TestSize) -> bool
	{
		if (!IsValidSlotPosition(StartSlotPosition))
			return false;

		if (StartSlotPosition.X + TestSize.X > GridSize.X || StartSlotPosition.Y + TestSize.Y > GridSize.Y)
			return false;

		for (int32 x = StartSlotPosition.X; x < StartSlotPosition.X + TestSize.X; x++)
		{
			for (int32 y = StartSlotPosition.Y; y < StartSlotPosition.Y + TestSize.Y; y++)
			{
				if (InvMatrix[x][y]->IsOccupied())
				{
					if (InvMatrix[x][y]->GetSlotItem() == IgnoreItem)
						continue;

					return false;
				}
			}
		}

		return true;
	};

	if (CheckSlots(Size))
		return true;

	if (bCheckRotation)
	{
		if (CheckSlots(FIntPoint(Size.Y, Size.X)))
		{
			bOutNeedsRotation = true;
			return true;
		}
	}

	return false;
}

bool UTetrisInventory::TryAddNewItemAtSlots(UTetrisItemData* ItemData, const FIntPoint StartSlotPosition)
{
	if (ItemData == nullptr || !CanContainItem(ItemData))
		return false;

	UTetrisItem* Item = Cast<UTetrisItem>(UISFactory::CreateSingleItem(this, ItemData));
	return TryOccupySlotsWithItem(Item, StartSlotPosition);
}

bool UTetrisInventory::TryAddItemAtSlots(UTetrisItem* Item, const FIntPoint StartSlotPosition)
{
	if (Item == nullptr || !CanContainItem(Item->GetItemData()))
		return false;

	return TryOccupySlotsWithItem(Item, StartSlotPosition);
}

bool UTetrisInventory::TryMoveItem(UTetrisItem* Item, const FIntPoint NewPosition, const bool bTryRotate, const UTetrisItem* IgnoreItem)
{
	if (!IsValidSlotPosition(NewPosition))
		return false;

	const FIntPoint OldPosition = Item->GetCurrentPosition();
	FreeSlots(Item->GetCurrentPosition(), Item->GetCachedSize()); // Get cached size because the item might be rotated during the drag

	if (TryOccupySlotsWithItem(Item, NewPosition, bTryRotate, IgnoreItem, false)) // Try to occupy the new slots
	{
		OnItemMoved.Broadcast(Item, OldPosition, NewPosition);
		return true;
	}

	Item->ResetToCachedRotation();
	OccupySlotsWithItem(Item, OldPosition, false); // Re-occupy the old slots if failed
	return true;
}

bool UTetrisInventory::TrySwitchItemsPositions(UTetrisItem* ItemA, UTetrisItem* ItemB)
{
	// if (ItemA->IsEquipped() || ItemB->IsEquipped()) // The equip switch is managed by the Equipment class
	// 	return false;
	//
	// const FIntPoint ItemAPosition = ItemA->GetCurrentPosition();
	// const FIntPoint ItemBPosition = ItemB->GetCurrentPosition();
	// const FIntPoint ItemASize = ItemA->GetCachedSize();
	// const FIntPoint ItemBSize = ItemB->GetCachedSize();
	//
	// bool bItemANeedsRotation = false;
	// bool bItemBNeedsRotation = false;
	// if (CanOccupySlots(ItemASize, ItemBPosition, bItemANeedsRotation, true, ItemB))
	// {
	// 	if (CanOccupySlots(ItemBSize, ItemAPosition, bItemBNeedsRotation, true, ItemA))
	// 	{
	// 		// ItemA->SetRotation(bItemANeedsRotation);
	// 		// ItemB->SetRotation(bItemBNeedsRotation);
	//
	// 		OccupySlotsWithItem(ItemA, ItemBPosition, false);
	// 		OccupySlotsWithItem(ItemB, ItemAPosition, false);
	// 		
	// 		OnItemsSwitchedPositions.Broadcast(ItemA, ItemB, ItemAPosition, ItemBPosition, ItemBPosition, ItemAPosition);
	// 		return true;
	// 	}
	// }

	// TODO: Implement the switch items positions
	return false;
}

void UTetrisInventory::BeginPlay()
{
	Super::BeginPlay();
	ConstructGrid();
}

void UTetrisInventory::OnItemAdded_Implementation(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UTetrisInventory::OnItemAdded_Implementation: Invalid Item added to inventory."));
		return;
	}

	const UTetrisItemData* TetrisItemData = Cast<UTetrisItemData>(Item->GetItemData());
	UTetrisItem* TetrisItem = Cast<UTetrisItem>(Item);

	FIntPoint FoundSlot;
	bool bNeedsToRotate;
	if (!FindAvailableSlots(TetrisItem->GetRelativeSize(), FoundSlot, bNeedsToRotate))
		return;

	if (bNeedsToRotate)
		TetrisItem->Rotate();

	OccupySlotsWithItem(TetrisItem, FoundSlot); // Since we found the slots, occupy them with the item
}

void UTetrisInventory::OnItemRemoved_Implementation(UItemBase* Item)
{
	UTetrisItem* TetrisItem = Cast<UTetrisItem>(Item);

	const int32 CurrX = TetrisItem->GetCurrentPosition().X;
	const int32 CurrY = TetrisItem->GetCurrentPosition().Y;

	FreeSlots(TetrisItem->GetCurrentPosition(), TetrisItem->GetCachedSize());
	OnItemRemovedFromSlot.Broadcast(TetrisItem, TetrisItem->GetCurrentPosition());
}

void UTetrisInventory::OnClearedInventory_Implementation()
{
	for (int32 x = 0; x < GridSize.X; x++)
	{
		for (int32 y = 0; y < GridSize.Y; y++)
			InvMatrix[x][y]->FreeSlot();
	}
}

void UTetrisInventory::ConstructGrid()
{
	Capacity = GridSize.X * GridSize.Y;
	InvMatrix = TArray<TArray<UTetrisSlot*>>();
	InvMatrix.SetNum(GridSize.X);
	for (int32 x = 0; x < GridSize.X; x++)
	{
		InvMatrix[x].SetNum(GridSize.Y);
		for (int32 y = 0; y < GridSize.Y; y++)
		{
			InvMatrix[x][y] = NewObject<UTetrisSlot>(this);
			InvMatrix[x][y]->Init(this, FIntPoint(x, y));
		}
	}
}

bool UTetrisInventory::IsValidSlotPosition(const FIntPoint SlotPosition) const
{
	return InvMatrix.IsValidIndex(SlotPosition.X) && InvMatrix[SlotPosition.X].IsValidIndex(SlotPosition.Y);
}

void UTetrisInventory::FreeSlots(const FIntPoint StartPosition, const FIntPoint Size)
{
	for (int32 x = StartPosition.X; x < StartPosition.X + Size.X; x++)
	{
		for (int32 y = StartPosition.Y; y < StartPosition.Y + Size.Y; y++)
		{
			if (IsValidSlotPosition(FIntPoint(x, y))) // Safe check
				InvMatrix[x][y]->FreeSlot();
		}
	}
}

void UTetrisInventory::OccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList)
{
	const FIntPoint ItemSize = Item->GetRelativeSize();

	for (int32 x = StartSlotPosition.X; x < StartSlotPosition.X + ItemSize.X; x++)
	{
		for (int32 y = StartSlotPosition.Y; y < StartSlotPosition.Y + ItemSize.Y; y++)
			InvMatrix[x][y]->OccupySlot(Item);
	}

	Item->SetCurrentPosition(StartSlotPosition);
	Item->CacheCurrentRotation();

	if (bAddItemToList)
	{
		OnItemAddedToSlot.Broadcast(Item, Item->GetCurrentPosition());
		AddItemToList(Item);
	}
}

bool UTetrisInventory::TryOccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bTryRotate, const UTetrisItem* IgnoreItem, const bool bAddItemToList)
{
	bool bNeedsRotation = false;
	if (!CanOccupySlots(Item->GetRelativeSize(), StartSlotPosition, bNeedsRotation, bTryRotate, IgnoreItem))
		return false;

	if (bNeedsRotation)
		Item->Rotate();

	OccupySlotsWithItem(Item, StartSlotPosition, bAddItemToList);
	return true;
}

bool UTetrisInventory::FindAvailableSlots(const FIntPoint Size, FIntPoint& OutStartSlotPosition, bool& OutbNeedsRotation) const
{
	auto TryFindSlot = [&](const FIntPoint& TestSize, const bool bRotationNeeded) -> bool
	{
		for (int32 x = 0; x <= GridSize.X - TestSize.X; x++)
		{
			for (int32 y = 0; y <= GridSize.Y - TestSize.Y; y++)
			{
				bool bNeedsRotation = false;
				if (CanOccupySlots(TestSize, FIntPoint(x, y), bNeedsRotation))
				{
					OutStartSlotPosition = FIntPoint(x, y);
					OutbNeedsRotation = bRotationNeeded;
					return true;
				}
			}
		}

		return false;
	};

	// Try both orientations: normal and rotated
	return TryFindSlot(Size, false) || TryFindSlot(FIntPoint(Size.Y, Size.X), true);
}
