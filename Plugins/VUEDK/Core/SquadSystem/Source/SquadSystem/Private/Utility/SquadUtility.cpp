// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/SquadUtility.h"

TWeakObjectPtr<USquadSubsystem> USquadUtility::SquadSubsystem = nullptr;

void USquadUtility::Init(USquadSubsystem* Subsystem)
{
	SquadSubsystem = Subsystem;
}

bool USquadUtility::ResolveJoin(USquadMember* Requester, USquadMember* Receiver, const ESquadResolution Resolution)
{
	if (!Check())
		return false;

	return SquadSubsystem.Get()->ResolveJoin(Requester, Receiver, Resolution);
}

bool USquadUtility::ResolveLeave(USquadMember* MemberToLeave, const ESquadResolution Resolution)
{
	if (!Check())
		return false;

	return SquadSubsystem.Get()->ResolveLeave(MemberToLeave, Resolution);
}

USquad* USquadUtility::SimpleCreateSquad(const TArray<USquadMember*>& Members, const FName SquadID)
{
	if (!Check())
		return nullptr;

	return SquadSubsystem.Get()->SimpleCreateSquad(Members, SquadID);
}

void USquadUtility::RemoveSquad(const FName SquadID)
{
	if (!Check())
		return;

	SquadSubsystem.Get()->RemoveSquad(SquadID);
}

void USquadUtility::ClearSquads()
{
	if (!Check())
		return;

	SquadSubsystem.Get()->ClearSquads();
}

int32 USquadUtility::GetMaxMembersPerSquad()
{
	if (!Check())
		return 0;

	return SquadSubsystem.Get()->GetMaxMembersPerSquad();
}

USquad* USquadUtility::FindSquad(const FName SquadID)
{
	if (!Check())
		return nullptr;

	return SquadSubsystem.Get()->FindSquad(SquadID);
}

FSquadInfo USquadUtility::GetSquadInfo(const FName SquadID)
{
	if (!Check())
		return FSquadInfo();

	return SquadSubsystem.Get()->GetSquadInfo(SquadID);
}

bool USquadUtility::Check()
{
	return SquadSubsystem.IsValid();
}
