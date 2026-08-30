// Copyright VUEDK, Inc. All Rights Reserved.

#include "Member/SquadMember.h"
#include "SquadSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Member/MemberEvaluator.h"
#include "Net/UnrealNetwork.h"
#include "Squad/Squad.h"
#include "Utility/SquadUtility.h"

USquadMember::USquadMember()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void USquadMember::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickMemberObjects(DeltaTime);
}

void USquadMember::ChangeLeadership(const bool bNewIsLeader)
{
	if (GetOwnerRole() != ROLE_Authority)
		return;

	bIsLeader = bNewIsLeader;
	if (IsValid(LeaderController))
		LeaderController->ChangeLeadership(bIsLeader);

	OnLeadershipChanged.Broadcast(bNewIsLeader);
}

void USquadMember::AssignSquad(USquad* NewSquad)
{
	if (GetOwnerRole() != ROLE_Authority)
		return;
	
	OwnerSquad = NewSquad;
	OwnerSquadID = NewSquad->GetSquadId();
	JoinMemberObjects(NewSquad);
	OnJoinedSquad.Broadcast(OwnerSquadID);
}

void USquadMember::ClearSquad()
{
	if (GetOwnerRole() != ROLE_Authority)
		return;
	
	USquad* LeftSquad = OwnerSquad;
	OwnerSquad = nullptr;
	OwnerSquadID = NAME_None;
	LeaveMemberObjects(LeftSquad);
	OnLeftSquad.Broadcast();
}

FName USquadMember::GetOwnerSquadID() const
{
	return OwnerSquadID;
}

bool USquadMember::HasSquad() const
{
	return OwnerSquadID != NAME_None;
}

bool USquadMember::IsLeader() const
{
	return bIsLeader;
}

USquad* USquadMember::GetOwnerSquad() const
{
	return OwnerSquad;
}

USquadMember* USquadMember::GetOwnerSquadLeader() const
{
	return IsValid(OwnerSquad) ? OwnerSquad->GetLeader() : nullptr;
}

bool USquadMember::CanBeLeader() const
{
	return bCanBeLeader && IsValid(LeaderController);
}

bool USquadMember::JoinSquad(USquadMember* Target, const TEnumAsByte<ESquadResolution> Resolution)
{
	if (GetOwnerRole() != ROLE_Authority)
		return false;

	return USquadUtility::ResolveJoin(this, Target, Resolution);
}

bool USquadMember::LeaveSquad(const TEnumAsByte<ESquadResolution> Resolution)
{
	if (GetOwnerRole() != ROLE_Authority)
		return false;

	return USquadUtility::ResolveLeave(this, Resolution);
}

void USquadMember::BeginPlay()
{
	Super::BeginPlay();
	Init();
	BeginPlayMemberObjects();
}

void USquadMember::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EndPlayMemberObjects(EndPlayReason);
}

void USquadMember::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USquadMember, OwnerSquadID);
	DOREPLIFETIME(USquadMember, bIsLeader);
}

void USquadMember::Init()
{
	if (!Check())
	{
		SetComponentTickEnabled(false);
		UE_LOG(LogSquadSystem, Warning, TEXT("USquadMember::Init: Missing required member objects on %s"), *GetOwner()->GetName());
		return;
	}

#if !UE_BUILD_SHIPPING
	DrawDebugMemberState();
#endif

	if (GetOwnerRole() != ROLE_Authority) // Do not process on clients
		SetComponentTickEnabled(false);

	MemberObjects.Add(MembershipHandler);
	MemberObjects.Add(MemberEvaluator);
	if (bCanBeLeader)
		MemberObjects.Add(LeaderController);

	InitMemberObjects();
}

void USquadMember::InitMemberObjects()
{
	for (UMemberObjectBase* MemberObject : MemberObjects)
	{
		if (IsValid(MemberObject))
			MemberObject->Init(this);
	}
}

void USquadMember::JoinMemberObjects(USquad* NewSquad) const
{
	for (UMemberObjectBase* MemberObject : MemberObjects)
	{
		if (IsValid(MemberObject))
			MemberObject->OnJoinSquad(NewSquad);
	}
}

void USquadMember::LeaveMemberObjects(USquad* LeftSquad) const
{
	for (UMemberObjectBase* MemberObject : MemberObjects)
	{
		if (IsValid(MemberObject))
			MemberObject->OnLeaveSquad(LeftSquad);
	}
}

void USquadMember::TickMemberObjects(const float DeltaTime) const
{
	for (UMemberObjectBase* MemberObject : MemberObjects)
	{
		if (IsValid(MemberObject))
			MemberObject->Tick(DeltaTime);
	}
}

void USquadMember::BeginPlayMemberObjects() const
{
	for (UMemberObjectBase* MemberObject : MemberObjects)
	{
		if (IsValid(MemberObject))
			MemberObject->BeginPlay();
	}
}

void USquadMember::EndPlayMemberObjects(const EEndPlayReason::Type EndPlayReason) const
{
	for (UMemberObjectBase* MemberObject : MemberObjects)
	{
		if (IsValid(MemberObject))
			MemberObject->EndPlay(EndPlayReason);
	}
}

bool USquadMember::Check() const
{
	return IsValid(MembershipHandler) && IsValid(MemberEvaluator);
}

void USquadMember::OnRep_SquadID() const
{
	if (OwnerSquadID != NAME_None)
		OnJoinedSquad.Broadcast(OwnerSquadID);
	else
		OnLeftSquad.Broadcast();
}

#if !UE_BUILD_SHIPPING
void USquadMember::DrawDebugMemberState() const
{
	const bool bDebug = FSquadSystemModule::CVarMemberStateDebug.GetValueOnGameThread();
	
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimerForNextTick(this, &USquadMember::DrawDebugMemberState);

	if (!bDebug)
		return;
	
	AActor* OwnerActor = GetOwner();
	if (OwnerActor->IsA(AController::StaticClass()))
		OwnerActor = Cast<AController>(OwnerActor)->GetPawn();
	
	if (!IsValid(OwnerActor))
		return;

	FString DebugString;
	if (GetOwnerRole() != ROLE_Authority)
	{
		DebugString = TEXT("  [ CLIENT - SQUAD MEMBER ]\n");
		DebugString += TEXT("===============================\n");

		DebugString += FString::Printf(
			TEXT("  Name       : %s\n"),
			*OwnerActor->GetName()
		);

		DebugString += FString::Printf(
			TEXT("  Squad Id        : %s\n"),
			*OwnerSquadID.ToString()
		);
	}
	else
	{
		DebugString += IsLeader() ? TEXT("[ SQUAD LEADER ]\n") : TEXT("[ SQUAD MEMBER ]\n");
		DebugString += TEXT("===============================\n");

		DebugString += FString::Printf(
			TEXT("  Name       : %s\n"),
			*OwnerActor->GetName()
		);

		DebugString += FString::Printf(
			TEXT("  Squad Id        : %s\n"),
			*OwnerSquadID.ToString()
		);

		DebugString += FString::Printf(
			TEXT("  Squad Leader : %s\n"),
			IsValid(GetOwnerSquadLeader()) ? *GetOwnerSquadLeader()->GetOwner()->GetName() : TEXT("None")
		);

		DebugString += FString::Printf(
			TEXT("  Squad Member Count: %d\n"),
			HasSquad() && IsValid(OwnerSquad) ? OwnerSquad->GetMembersCount() : 0
		);

		DebugString += FString::Printf(
			TEXT("  Is Leader   : %s\n"),
			IsLeader() ? TEXT("True") : TEXT("False")
		);

		DebugString += FString::Printf(
			TEXT("  Can Lead    : %s\n\n"),
			CanBeLeader() ? TEXT("True") : TEXT("False")
		);
	}
	
	DebugString += TEXT("===============================");
	DrawDebugString(
		World,
		FVector(0.f, 0.f, 120.f), // Offset above actor
		DebugString,
		OwnerActor,
		FColor::White,
		0.f,
		true,
		1.f
	);
}
#endif
