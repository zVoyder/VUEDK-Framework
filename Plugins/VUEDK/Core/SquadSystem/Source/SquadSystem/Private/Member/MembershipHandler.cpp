// Copyright VUEDK, Inc. All Rights Reserved.

#include "Member/MembershipHandler.h"
#include "Squad/Squad.h"
#include "Utility/SquadUtility.h"

void UMembershipHandler::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwnerMember()->HasSquad())
	{
		TEnumAsByte<ESquadResolution> Resolution;
		if (!EvaluateMembershipValidity(Resolution))
			GetOwnerMember()->LeaveSquad(Resolution);
	}
}

void UMembershipHandler::SendJoinRequestTo(USquadMember* Target) const
{
	if (!IsValid(Target))
		return;

	TEnumAsByte<ESquadResolution> Resolution;
	USquadMember* Counterpart = nullptr; // The member who will respond on behalf of the target (could be the target itself or its leader)
	if (NegotiateJoinWith(Target, Counterpart, Resolution))
		GetOwnerMember()->JoinSquad(Counterpart, Resolution);
}

bool UMembershipHandler::EvaluateJoinRequestFrom_Implementation(USquadMember* Target, TEnumAsByte<ESquadResolution>& OutResolution)
{
	return true;
}

bool UMembershipHandler::EvaluateMembershipValidity_Implementation(TEnumAsByte<ESquadResolution>& OutResolution)
{
	return true;
}

bool UMembershipHandler::NegotiateJoinWith(USquadMember* Target, USquadMember*& OutCounterpart, TEnumAsByte<ESquadResolution>& OutResolution) const
{
	OutCounterpart = Target;
	UMembershipHandler* TargetHandler = Target->MembershipHandler;
	if (!IsValid(TargetHandler))
		return false;

	if (Target->HasSquad())
	{
		const USquad* OwnerSquad = GetOwnerSquad();
		const USquad* TargetSquad = Target->GetOwnerSquad();
		
		if (IsValid(OwnerSquad) && TargetSquad == OwnerSquad)
			return false; // Already in the same squad

		if (TargetSquad->IsLocked())
			return false; // Target squad is locked
		
		if (TargetSquad->HasLeader())
		{
			OutCounterpart = TargetSquad->GetLeader();
			return OutCounterpart->MembershipHandler->EvaluateJoinRequestFrom(GetOwnerMember(), OutResolution);
		}
	}

	return TargetHandler->EvaluateJoinRequestFrom(GetOwnerMember(), OutResolution);
}
