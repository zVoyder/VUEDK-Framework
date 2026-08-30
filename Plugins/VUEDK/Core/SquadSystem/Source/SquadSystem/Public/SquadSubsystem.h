// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Member/SquadMember.h"
#include "SquadEvaluator.h"
#include "Squad/Squad.h"
#include "Subsystems/WorldSubsystem.h"
#include "SquadSubsystem.generated.h"

class USquad;
class USquadSystemSettings;

UENUM()
enum EDefaultResolution
{
	DefaultDirect,
	DefaultRebuild
};

UCLASS()
class SQUADSYSTEM_API USquadSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	const USquadSystemSettings* SquadSystemSettings;
	UPROPERTY()
	TSet<USquad*> Squads;
	UPROPERTY()
	USquadEvaluator* Evaluator;
	int32 MaxMembersPerSquad;
	TEnumAsByte<EDefaultResolution> DefaultJoinResolution;
	float AccumulatedTime;
	float TickInterval;

public:
	USquadSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

	USquad* SimpleCreateSquad(const TArray<USquadMember*>& Members, const FName SquadId);

	void RemoveSquad(const FName SquadId);

	void ClearSquads();
	
	USquad* FindSquad(const FName SquadId) const;

	bool HasSquad(const FName SquadId) const;
	
	bool ResolveJoin(USquadMember* Requester, USquadMember* Receiver, const ESquadResolution Resolution);
	
	bool ResolveLeave(USquadMember* MemberToLeave, const ESquadResolution Resolution);

	int32 GetMaxMembersPerSquad() const;

	FSquadInfo GetSquadInfo(const FName SquadId) const;

private:
	bool AssignBestLeader(USquad* Squad) const;
	
	USquad* CreateAndAddSquad(const TArray<USquadMember*>& Members, const FName SquadId = FName(*FGuid::NewGuid().ToString()));
	
	bool DirectJoin(USquadMember* Requester, USquadMember* Receiver);

	bool DirectLeave(USquadMember* MemberToLeave) const;

	bool JoinSingleToSquad(USquadMember* Requester, USquad* TargetSquad) const;
	
	bool JoinTwoSquads(USquad* SquadRequester, USquad* SquadReceiver);
	
	void FormSquads(const TArray<USquadMember*>& Members);
	
	void CreateBestSquads(const TArray<USquadMember*>& AllMembers);

	void ClearSquads(const TArray<USquadMember*>& AllMembers);
	
	void AddSquad(USquad* NewSquad, const TArray<USquadMember*>& SquadMembers);
	
	void RemoveSquad(USquad* SquadToRemove);

	static void AddMembers(USquad* SquadToAdd, const TArray<USquadMember*>& MembersToAdd);

	static void RemoveMembers(USquad* SquadToRemove);
	
	static TArray<USquadMember*> GetAllMembersFrom(USquadMember* MemberA, USquadMember* MemberB);

	UFUNCTION()
	void OnSquadMemberJoin(USquad* Squad, USquadMember* Member);

	UFUNCTION()
	void OnSquadMemberLeave(USquad* Squad, USquadMember* Member);
};
