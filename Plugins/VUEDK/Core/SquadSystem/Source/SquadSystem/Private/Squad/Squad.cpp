// Copyright VUEDK, Inc. All Rights Reserved.

#include "Squad/Squad.h"
#include "SquadSubsystem.h"
#include "SquadSystem.h"
#include "Member/SquadMember.h"
#include "Utility/SquadUtility.h"

void USquad::Init(const USquadSubsystem* InSquadSubsystem, const FName InSquadID, const bool bLockedState)
{
	SquadSubsystem = InSquadSubsystem;
	SquadID = InSquadID;
	bIsLocked = bLockedState;
#if !UE_BUILD_SHIPPING
	DrawDebugSquadTrace();
#endif
}

void USquad::TickSquad(const float DeltaTime)
{
	TickSquadHandlers(DeltaTime);
}

void USquad::AddHandler(USquadHandler* NewHandler)
{
	Handlers.Add(NewHandler);
	NewHandler->OnAddedToSquad(this);
}

void USquad::RemoveHandler(USquadHandler* HandlerToRemove)
{
	if (!Handlers.Contains(HandlerToRemove))
		return;
	
	Handlers.Remove(HandlerToRemove);
	HandlerToRemove->Deinit();
	HandlerToRemove->OnRemovedFromSquad(this);
}

void USquad::ClearHandlers()
{
	for (USquadHandler* Handler : Handlers)
	{
		if (IsValid(Handler))
			Handler->Deinit();
	}

	Handlers.Empty();
}

FName USquad::GetSquadId() const
{
	return SquadID;
}

void USquad::Lock()
{
	bIsLocked = true;
}

void USquad::Unlock()
{
	bIsLocked = false;
}

bool USquad::IsLocked() const
{
	return bIsLocked;
}

ESquadHandlerResult USquad::GetSquadHandler(const TSubclassOf<USquadHandler> HandlerClass, USquadHandler*& OutHandler) const
{
	for (USquadHandler* Handler : Handlers)
	{
		if (Handler->IsA(HandlerClass))
		{
			OutHandler = Handler;
			return ESquadHandlerResult::Valid;
		}
	}

	OutHandler = nullptr;
	return ESquadHandlerResult::NotValid;
}

USquadMember* USquad::GetLeader() const
{
	return Leader;
}

ULeaderController* USquad::GetLeaderController() const
{
	return IsValid(Leader) ? Leader->LeaderController : nullptr;
}

bool USquad::HasLeader() const
{
	return IsValid(Leader);
}

const TArray<USquadMember*>& USquad::GetMembers() const
{
	return Members;
}

int32 USquad::GetMembersCount() const
{
	return Members.Num();
}

bool USquad::IsFull() const
{
	return Members.Num() >= USquadUtility::GetMaxMembersPerSquad();
}

bool USquad::CanContainsMoreMembers(const int32 AdditionalMembers) const
{
	return (Members.Num() + AdditionalMembers) <= USquadUtility::GetMaxMembersPerSquad();
}

bool USquad::ContainsMember(USquadMember* Member) const
{
	return Members.Contains(Member);
}

UWorld* USquad::GetWorld() const
{
	return IsValid(SquadSubsystem) ? SquadSubsystem->GetWorld() : nullptr;
}

void USquad::AssignLeader(USquadMember* NewLeader)
{
	if (NewLeader == Leader)
		return;

	if (!Members.Contains(NewLeader))
	{
		UE_LOG(LogSquadSystem, Warning, TEXT("USquad::AssignLeader: The member is not part of the squad."));
		return;
	}

	RemoveLeader();
	Leader = NewLeader;
	Leader->ChangeLeadership(true);

	for (USquadMember* Member : Members)
	{
		if (IsValid(Member) && Member != Leader)
			Member->ChangeLeadership(false);
	}
}

void USquad::RemoveLeader()
{
	if (!IsValid(Leader))
		return;

	for (USquadMember* Member : Members)
	{
		if (IsValid(Member))
			Member->ChangeLeadership(false);
	}

	Leader = nullptr;
}

bool USquad::TryAddMember(USquadMember* InMember, const bool bNotify)
{
	if (!CanContainsMoreMembers())
		return false;

	AddMember(InMember, bNotify);
	return true;
}

bool USquad::TryAddMembers(const TArray<USquadMember*>& InMembers, const bool bNotify)
{
	if (!CanContainsMoreMembers(InMembers.Num()))
		return false;

	for (USquadMember* Member : InMembers)
		AddMember(Member, bNotify);

	return true;
}

void USquad::AddMember(USquadMember* InMember, const bool bNotify)
{
	if (!IsValid(InMember))
		return;

	Members.Add(InMember);
	InMember->AssignSquad(this);

	if (bNotify)
		OnSquadMemberJoined.Broadcast(this, InMember);
}

void USquad::RemoveMember(USquadMember* InMember, const bool bNotify)
{
	if (!IsValid(InMember))
		return;

	InMember->ClearSquad();
	Members.Remove(InMember);

	if (bNotify)
		OnSquadMemberLeft.Broadcast(this, InMember);
}

void USquad::TickSquadHandlers(const float DeltaTime)
{
	for (USquadHandler* Handler : Handlers)
	{
		if (IsValid(Handler))
			Handler->Tick(DeltaTime);
	}
}

void USquad::BeginPlaySquadHandlers()
{
	for (USquadHandler* Handler : Handlers)
	{
		if (IsValid(Handler))
			Handler->BeginPlay();
	}
}

void USquad::EndPlaySquadHandlers()
{
	for (USquadHandler* Handler : Handlers)
	{
		if (IsValid(Handler))
			Handler->EndPlay();
	}
}

#if !UE_BUILD_SHIPPING
void USquad::DrawDebugSquadTrace() const
{
	const bool bDebug = FSquadSystemModule::CVarSquadTraceDebug.GetValueOnGameThread();

	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimerForNextTick(this, &USquad::DrawDebugSquadTrace);

	if (!bDebug)
		return;

	for (int i = 0; i < Members.Num(); i++)
	{
		const USquadMember* MemberA = Members[i];
		if (!IsValid(MemberA))
			continue;

		const AActor* ActorA = MemberA->GetOwner();
		if (ActorA->IsA(AController::StaticClass()))
			ActorA = Cast<AController>(ActorA)->GetPawn();

		if (!IsValid(ActorA))
			continue;

		for (int j = i + 1; j < Members.Num(); j++)
		{
			const USquadMember* MemberB = Members[j];
			if (!IsValid(MemberB))
				continue;

			const AActor* ActorB = MemberB->GetOwner();
			if (ActorB->IsA(AController::StaticClass()))
				ActorB = Cast<AController>(ActorB)->GetPawn();

			if (!IsValid(ActorB))
				continue;

			FVector Start = ActorA->GetActorLocation();
			FVector End = ActorB->GetActorLocation();

			DrawDebugLine(
				World,
				Start,
				End,
				MemberB->IsLeader() || MemberA->IsLeader() ? FColor::Yellow : FColor::White,
				false,
				0.f,
				0,
				2.f
			);
		}
	}
}
#endif
