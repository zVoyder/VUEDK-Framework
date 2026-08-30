// Copyright VUEDK, Inc. All Rights Reserved.

#include "BetterUIEventsHandlerSubsystem.h"

void UBetterUIEventsHandlerSubsystem::RequestBack() const
{
	OnRequestBack.Broadcast();
}

void UBetterUIEventsHandlerSubsystem::RequestConfirm() const
{
	OnRequestConfirm.Broadcast();
}

void UBetterUIEventsHandlerSubsystem::RequestRightSelect() const
{
	OnRequestRightSelect.Broadcast();
}

void UBetterUIEventsHandlerSubsystem::RequestLeftSelect() const
{
	OnRequestLeftSelect.Broadcast();
}
