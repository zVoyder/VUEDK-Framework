// Copyright VUEDK, Inc. All Rights Reserved.

#include "Member/LeaderController.h"

void ULeaderController::ChangeLeadership(const bool bIsLeader)
{
	if (bIsLeader)
		OnAssumeLeadership();
	else
		OnRemoveLeadership();
}

float ULeaderController::EvaluateLeadership_Implementation(const TArray<USquadMember*>& Members) const
{
	return 0.0f;
}

void ULeaderController::OnAssumeLeadership_Implementation()
{
}

void ULeaderController::OnRemoveLeadership_Implementation()
{
}
