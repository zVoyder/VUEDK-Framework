// Copyright VUEDK, Inc. All Rights Reserved.

#include "Member/Base/MemberObjectBase.h"
#include "Member/SquadMember.h"
#include "Squad/Squad.h"

void UMemberObjectBase::Init(USquadMember* InOwnerMember)
{
	OwnerMember = InOwnerMember;
	OnInit(InOwnerMember);
}

void UMemberObjectBase::Tick(float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

void UMemberObjectBase::BeginPlay()
{
	ReceiveBeginPlay();
}

void UMemberObjectBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ReceiveEndPlay(EndPlayReason);
}

USquadMember* UMemberObjectBase::GetOwnerMember() const
{
	return OwnerMember;
}

AActor* UMemberObjectBase::GetOwnerActor() const
{
	return IsValid(OwnerMember) ? OwnerMember->GetOwner() : nullptr;
}

USquad* UMemberObjectBase::GetOwnerSquad() const
{
	return IsValid(OwnerMember) ? OwnerMember->GetOwnerSquad() : nullptr;
}

USquadMember* UMemberObjectBase::GetOwnerSquadLeader() const
{
	return IsValid(OwnerMember) ? OwnerMember->GetOwnerSquadLeader() : nullptr;
}

void UMemberObjectBase::OnJoinSquad_Implementation(USquad* NewSquad)
{
}

void UMemberObjectBase::OnLeaveSquad_Implementation(USquad* OldSquad)
{
}

#if WITH_EDITOR
bool UMemberObjectBase::ImplementsGetWorld() const
{
	return UObject::ImplementsGetWorld();
}
#endif

UWorld* UMemberObjectBase::GetWorld() const
{
	return IsValid(OwnerMember) ? OwnerMember->GetWorld() : nullptr;
}

void UMemberObjectBase::OnInit_Implementation(USquadMember* InOwnerMember)
{
}
