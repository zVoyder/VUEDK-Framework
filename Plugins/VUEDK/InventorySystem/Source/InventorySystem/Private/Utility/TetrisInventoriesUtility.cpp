// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/TetrisInventoriesUtility.h"
#include "TetrisInventory/TetrisInventoriesManager.h"
#include "TetrisInventory/TetrisInventoryEventsHandlerSubsystem.h"
#include "Utility/ISInventoriesUtility.h"

UTetrisInventoryEventsHandlerSubsystem* UTetrisInventoriesUtility::EventsHandlerSubsystem = nullptr;

void UTetrisInventoriesUtility::Init(UTetrisInventoryEventsHandlerSubsystem* InEventsHandlerSubsystem)
{
	EventsHandlerSubsystem = InEventsHandlerSubsystem;
}

UTetrisInventoriesManager* UTetrisInventoriesUtility::GetTetrisInventoriesManager()
{
	return Cast<UTetrisInventoriesManager>(UISInventoriesUtility::GetInventoriesManager());
}

bool UTetrisInventoriesUtility::TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory)
{
	return GetTetrisInventoriesManager()->TryTransferItem(Item, DestinationInventory);
}

bool UTetrisInventoriesUtility::TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition)
{
	return GetTetrisInventoriesManager()->TryTransferItemAtPosition(Item, DestinationInventory, DestinationPosition);
}

bool UTetrisInventoriesUtility::TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB)
{
	return GetTetrisInventoriesManager()->TryTransferSwitchItems(ItemA, ItemB);
}

void UTetrisInventoriesUtility::RequestDrag()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestDrag();
}

void UTetrisInventoriesUtility::RequestDrop()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestDrop();
}

void UTetrisInventoriesUtility::RequestRotate()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestRotate();
}

void UTetrisInventoriesUtility::RequestEquipUnequip(const int32 SwapPriority)
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestEquipUnequip(SwapPriority);
}

void UTetrisInventoriesUtility::RequestTransferItemToTargetInventory()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestTransferItemToTargetInventory();
}

void UTetrisInventoriesUtility::RequestDiscard()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestDiscard();
}

void UTetrisInventoriesUtility::RequestSplit()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestSplit();
}

void UTetrisInventoriesUtility::RequestCancelSplit()
{
	if (!Check())
		return;

	EventsHandlerSubsystem->RequestCancelSplit();
}

bool UTetrisInventoriesUtility::IsSplitting()
{
	if (!Check())
		return false;

	return EventsHandlerSubsystem->IsSplitting();
}

bool UTetrisInventoriesUtility::Check()
{
	return IsValid(EventsHandlerSubsystem);
}
