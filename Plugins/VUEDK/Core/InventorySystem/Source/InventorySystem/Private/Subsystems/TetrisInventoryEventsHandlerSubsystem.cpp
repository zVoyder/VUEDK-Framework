// Copyright VUEDK, Inc. All Rights Reserved.

#include "Subsystems/TetrisInventoryEventsHandlerSubsystem.h"
#include "Utility/TetrisInventoriesUtility.h"

void UTetrisInventoryEventsHandlerSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);
	PlayerController = NewPlayerController;
	UTetrisInventoriesUtility::SetTetrisEventsHandler(this);
}

void UTetrisInventoryEventsHandlerSubsystem::RequestDrag() const
{
	OnRequestDrag.Broadcast();
}

void UTetrisInventoryEventsHandlerSubsystem::RequestDrop() const
{
	OnRequestDrop.Broadcast();
}

void UTetrisInventoryEventsHandlerSubsystem::RequestRotate() const
{
	OnRequestRotate.Broadcast();
}

void UTetrisInventoryEventsHandlerSubsystem::RequestEquipUnequip(const int32 SwapPriority) const
{
	OnRequestEquipUnequip.Broadcast(SwapPriority);
}

void UTetrisInventoryEventsHandlerSubsystem::RequestTransferItemToTargetInventory() const
{
	OnRequestTransferItemToTargetInventory.Broadcast();
}

void UTetrisInventoryEventsHandlerSubsystem::RequestDiscard() const
{
	OnRequestDiscard.Broadcast(PlayerController);
}

void UTetrisInventoryEventsHandlerSubsystem::RequestSplit()
{
	bIsSplitting = true;
	OnRequestSplit.Broadcast();
}

void UTetrisInventoryEventsHandlerSubsystem::RequestCancelSplit()
{
	bIsSplitting = false;
	OnRequestCancelSplit.Broadcast();
}

bool UTetrisInventoryEventsHandlerSubsystem::IsSplitting() const
{
	return bIsSplitting;
}
