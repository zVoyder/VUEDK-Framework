// Copyright VUEDK, Inc. All Rights Reserved.

#include "TetrisInventory/TetrisSlot.h"

void UTetrisSlot::Init(UTetrisInventory* InInventory, const FIntPoint InSlotPosition)
{
	RelatedInventory = InInventory;
	SlotPosition = InSlotPosition;
}

void UTetrisSlot::OccupySlot(UTetrisItem* InItem)
{
	bIsOccupied = true;
	Item = InItem;
}

void UTetrisSlot::FreeSlot()
{
	bIsOccupied = false;
	Item = nullptr;
}

bool UTetrisSlot::IsOccupied() const
{
	return bIsOccupied;
}

UTetrisItem* UTetrisSlot::GetSlotItem() const
{
	return Item;
}

FIntPoint UTetrisSlot::GetSlotPosition() const
{
	return SlotPosition;
}
