// Copyright VUEDK, Inc. All Rights Reserved.

#include "SquadSubsystem.h"
#include "SquadSystem.h"
#include "SquadSystemSettings.h"
#include "Factories/SquadFactory.h"
#include "Squad/Squad.h"
#include "Utility/SquadUtility.h"

USquadSubsystem::USquadSubsystem()
{

}

void USquadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SquadSystemSettings = GetDefault<USquadSystemSettings>();
	if (!IsValid(SquadSystemSettings))
	{
		UE_LOG(LogSquadSystem, Error, TEXT("USquadSubsystem::Initialize: SquadSystemSettings is invalid."));
		return;
	}

	DefaultJoinResolution = SquadSystemSettings->DefaultJoinResolution;
	MaxMembersPerSquad = SquadSystemSettings->MaxMembersPerSquad;
	TickInterval = SquadSystemSettings->SquadTickInterval;
	const TSubclassOf<USquadEvaluator> EvaluatorClass = SquadSystemSettings->SquadEvaluatorClass.LoadSynchronous();
	Evaluator = USquadFactory::CreateSquadEvaluator(this, EvaluatorClass);
	USquadUtility::Init(this);
}

void USquadSubsystem::Tick(float DeltaTime)
{
	AccumulatedTime += DeltaTime;
	if (AccumulatedTime < TickInterval)
		return;

	const float Step = AccumulatedTime;
	AccumulatedTime = 0.f;
	
	for (USquad* Squad : Squads)
	{
		if (IsValid(Squad))
			Squad->TickSquad(DeltaTime);
	}
}

bool USquadSubsystem::IsTickable() const
{
	return !Squads.IsEmpty();
}

TStatId USquadSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USquadSubsystem, STATGROUP_Tickables);
}

USquad* USquadSubsystem::SimpleCreateSquad(const TArray<USquadMember*>& Members, const FName SquadId)
{
	if (HasSquad(SquadId))
		return nullptr;

	return CreateAndAddSquad(Members, SquadId);
}

void USquadSubsystem::RemoveSquad(const FName SquadId)
{
	RemoveSquad(FindSquad(SquadId));
}

void USquadSubsystem::ClearSquads()
{
	for (USquad* Squad : Squads)
	{
		if (IsValid(Squad))
			RemoveSquad(Squad);
	}
}

USquad* USquadSubsystem::FindSquad(const FName SquadId) const
{
	for (USquad* Squad : Squads)
	{
		if (IsValid(Squad) && Squad->GetSquadId() == SquadId)
			return Squad;
	}

	return nullptr;
}

bool USquadSubsystem::HasSquad(const FName SquadId) const
{
	return IsValid(FindSquad(SquadId));
}

bool USquadSubsystem::ResolveJoin(USquadMember* Requester, USquadMember* Receiver, const ESquadResolution Resolution)
{
	if (!IsValid(Requester) || !IsValid(Receiver))
		return false;

	USquad* TargetSquad = Receiver->GetOwnerSquad();
	const TArray<USquadMember*> AllMembers = GetAllMembersFrom(Requester, Receiver);

	auto ApplyDefaultResolution = [&]()
	{
		if (DefaultJoinResolution == EDefaultResolution::DefaultDirect)
			return DirectJoin(Requester, Receiver);

		if (DefaultJoinResolution == EDefaultResolution::DefaultRebuild)
		{
			FormSquads(AllMembers);
			return true;
		}
		return false;
	};

	switch (Resolution)
	{
	case ESquadResolution::Direct:
		return DirectJoin(Requester, Receiver);

	case ESquadResolution::Rebuild:
		FormSquads(AllMembers);
		return true;

	case ESquadResolution::DirectPreferred:
		if (DirectJoin(Requester, Receiver))
			return true;
		return ApplyDefaultResolution();

	case ESquadResolution::DefaultPreferred:
		return ApplyDefaultResolution();

	default:
		return false;
	}
}

bool USquadSubsystem::ResolveLeave(USquadMember* MemberToLeave, const ESquadResolution Resolution)
{
	if (!IsValid(MemberToLeave) || !MemberToLeave->HasSquad())
		return false;

	const USquad* OwnerSquad = MemberToLeave->GetOwnerSquad();
	TArray<USquadMember*> Members = OwnerSquad->GetMembers();
	Members.Remove(MemberToLeave);

	auto ApplyDefaultResolution = [&]()
	{
		if (DefaultJoinResolution == EDefaultResolution::DefaultDirect)
			return DirectLeave(MemberToLeave);

		if (DefaultJoinResolution == EDefaultResolution::DefaultRebuild)
		{
			FormSquads(Members);
			return true;
		}
		return false;
	};

	switch (Resolution)
	{
	case ESquadResolution::Direct:
		return DirectLeave(MemberToLeave);

	case ESquadResolution::Rebuild:
		FormSquads(Members);
		return true;

	case ESquadResolution::DirectPreferred:
		if (DirectLeave(MemberToLeave))
			return true;
		return ApplyDefaultResolution();

	case ESquadResolution::DefaultPreferred:
		return ApplyDefaultResolution();

	default:
		return false;
	}
}

int32 USquadSubsystem::GetMaxMembersPerSquad() const
{
	return MaxMembersPerSquad;
}

FSquadInfo USquadSubsystem::GetSquadInfo(const FName SquadId) const
{
	FSquadInfo SquadInfo;
	const USquad* Squad = FindSquad(SquadId);

	if (IsValid(Squad))
		SquadInfo = FSquadInfo(Squad);

	return SquadInfo;
}

bool USquadSubsystem::AssignBestLeader(USquad* Squad) const
{
	USquadMember* Leader = Evaluator->FindBestLeader(Squad->GetMembers());
	if (!IsValid(Leader))
		return false;

	Squad->AssignLeader(Leader);
	return true;
}

USquad* USquadSubsystem::CreateAndAddSquad(const TArray<USquadMember*>& Members, const FName SquadId)
{
	if (Members.Num() <= 1 || Members.Num() > MaxMembersPerSquad)
		return nullptr;

	USquad* NewSquad = USquadFactory::CreateSquad(this, SquadId, SquadSystemSettings->bDefaultSquadLockedState, SquadSystemSettings->SquadHandlerClasses);
	AddSquad(NewSquad, Members);
	return NewSquad;
}

bool USquadSubsystem::DirectJoin(USquadMember* Requester, USquadMember* Receiver)
{
	if (!IsValid(Requester) || !IsValid(Receiver))
		return false;

	if (Requester->HasSquad() && Receiver->HasSquad())
	{
		USquad* RequesterSquad = Requester->GetOwnerSquad();
		USquad* ReceiverSquad = Receiver->GetOwnerSquad();

		if (RequesterSquad == ReceiverSquad)
			return false; // Already in the same squad

		return JoinTwoSquads(RequesterSquad, ReceiverSquad);
	}

	if (!Requester->HasSquad() && Receiver->HasSquad())
	{
		USquad* TargetSquad = Receiver->GetOwnerSquad();
		return JoinSingleToSquad(Requester, TargetSquad);
	}

	if (Requester->HasSquad() && !Receiver->HasSquad())
	{
		USquad* TargetSquad = Requester->GetOwnerSquad();
		return JoinSingleToSquad(Receiver, TargetSquad);
	}

	return IsValid(CreateAndAddSquad({Requester, Receiver}));
}

bool USquadSubsystem::DirectLeave(USquadMember* MemberToLeave) const
{
	const bool bIsLeader = MemberToLeave->IsLeader();
	USquad* OwnerSquad = MemberToLeave->GetOwnerSquad();
	OwnerSquad->RemoveMember(MemberToLeave);

	if (bIsLeader)
		OwnerSquad->RemoveLeader();

	AssignBestLeader(OwnerSquad);
	return true;
}

bool USquadSubsystem::JoinSingleToSquad(USquadMember* Requester, USquad* TargetSquad) const
{
	if (!IsValid(Requester) || !IsValid(TargetSquad))
		return false;

	if (TargetSquad->TryAddMember(Requester))
	{
		AssignBestLeader(TargetSquad);
		return true;
	}

	return false;
}

bool USquadSubsystem::JoinTwoSquads(USquad* SquadRequester, USquad* SquadReceiver)
{
	if (!IsValid(SquadRequester) || !IsValid(SquadReceiver))
		return false;

	if (SquadReceiver->CanContainsMoreMembers(SquadRequester->GetMembersCount()))
	{
		const TArray<USquadMember*> MembersToMove = SquadRequester->GetMembers();
		if (!SquadReceiver->TryAddMembers(MembersToMove))
			return false;

		RemoveSquad(SquadRequester);
		AssignBestLeader(SquadReceiver);
		return true;
	}

	if (SquadRequester->CanContainsMoreMembers(SquadReceiver->GetMembersCount()))
	{
		const TArray<USquadMember*> MembersToMove = SquadReceiver->GetMembers();
		if (!SquadRequester->TryAddMembers(MembersToMove))
			return false;

		RemoveSquad(SquadReceiver);
		AssignBestLeader(SquadRequester);
		return true;
	}

	return false;
}

void USquadSubsystem::FormSquads(const TArray<USquadMember*>& Members)
{
	ClearSquads(Members); // Clear existing squads before forming new ones
	CreateBestSquads(Members); // Create new squads
}

void USquadSubsystem::CreateBestSquads(const TArray<USquadMember*>& AllMembers)
{
	FHypotheticalFormation SquadsFormation = Evaluator->FindBestFormation(AllMembers, MaxMembersPerSquad);

	for (const FHypotheticalSquad& HypotheticalSquad : SquadsFormation.Squads)
	{
		TArray<USquadMember*> SquadMembers = HypotheticalSquad.Members;
		CreateAndAddSquad(SquadMembers);
	}
}

void USquadSubsystem::ClearSquads(const TArray<USquadMember*>& AllMembers)
{
	TSet<USquad*> SquadsToRemove;
	for (const USquadMember* Member : AllMembers)
	{
		if (!IsValid(Member))
			continue;
		
		if (USquad* OwnerSquad = Member->GetOwnerSquad())
			SquadsToRemove.Add(OwnerSquad);
	}

	for (USquad* Squad : SquadsToRemove)
		RemoveSquad(Squad);
}

void USquadSubsystem::AddSquad(USquad* NewSquad, const TArray<USquadMember*>& SquadMembers)
{
	if (!IsValid(NewSquad) || Squads.Contains(NewSquad))
		return;

	Squads.Add(NewSquad);
	AddMembers(NewSquad, SquadMembers);
	AssignBestLeader(NewSquad);
	NewSquad->BeginPlaySquadHandlers();
	NewSquad->OnSquadMemberJoined.AddUniqueDynamic(this, &USquadSubsystem::OnSquadMemberJoin);
	NewSquad->OnSquadMemberLeft.AddUniqueDynamic(this, &USquadSubsystem::OnSquadMemberLeave);
}

void USquadSubsystem::RemoveSquad(USquad* SquadToRemove)
{
	if (!IsValid(SquadToRemove))
		return;

	SquadToRemove->EndPlaySquadHandlers();
	SquadToRemove->ClearHandlers();
	RemoveMembers(SquadToRemove);
	Squads.Remove(SquadToRemove);
	SquadToRemove->OnSquadMemberJoined.RemoveDynamic(this, &USquadSubsystem::OnSquadMemberJoin);
	SquadToRemove->OnSquadMemberLeft.RemoveDynamic(this, &USquadSubsystem::OnSquadMemberLeave);
}

void USquadSubsystem::AddMembers(USquad* SquadToAdd, const TArray<USquadMember*>& MembersToAdd)
{
	for (USquadMember* Member : MembersToAdd)
		SquadToAdd->AddMember(Member, false);
}

void USquadSubsystem::RemoveMembers(USquad* SquadToRemove)
{
	TArray<USquadMember*> Members = SquadToRemove->GetMembers();
	for (USquadMember* Member : Members)
		SquadToRemove->RemoveMember(Member, false);
}

TArray<USquadMember*> USquadSubsystem::GetAllMembersFrom(USquadMember* MemberA, USquadMember* MemberB)
{
	TSet<USquadMember*> AllMembers;
	AllMembers.Add(MemberA);
	AllMembers.Add(MemberB);
	const USquad* SquadA = MemberA->GetOwnerSquad();
	const USquad* SquadB = MemberB->GetOwnerSquad();

	if (IsValid(SquadA))
	{
		for (USquadMember* Member : SquadA->GetMembers())
			AllMembers.Add(Member);
	}

	if (IsValid(SquadB))
	{
		for (USquadMember* Member : SquadB->GetMembers())
			AllMembers.Add(Member);
	}

	return AllMembers.Array();
}

void USquadSubsystem::OnSquadMemberJoin(USquad* Squad, USquadMember* Member)
{
}

void USquadSubsystem::OnSquadMemberLeave(USquad* Squad, USquadMember* Member)
{
	if (Squad->GetMembersCount() <= 1)
		RemoveSquad(Squad);
}
