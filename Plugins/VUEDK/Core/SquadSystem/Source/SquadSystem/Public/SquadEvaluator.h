// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Member/SquadMember.h"
#include "UObject/Object.h"
#include "SquadEvaluator.generated.h"

/**
 * Represents a hypothetical squad consisting of multiple members.
 */
struct FHypotheticalSquad
{
	float Score;
	uint32 Mask;
	TArray<USquadMember*> Members;

	explicit FHypotheticalSquad(const uint32 InMask, const TArray<USquadMember*>& InMembers) : Score(0),
	                                                                                           Mask(InMask),
	                                                                                           Members(InMembers)
	{
	}
};

/**
 * Represents a hypothetical formation consisting of multiple squads.
 */
struct FHypotheticalFormation
{
	float Score;
	TArray<FHypotheticalSquad> Squads;

	FHypotheticalFormation() : Score(0)
	{
	}
};

UENUM(BlueprintType)
enum class EFormationPreference : uint8
{
	PreferFewerSquads,
	PreferMoreSquads,
	Balanced,
	Random
};

UCLASS(Blueprintable, BlueprintType)
class SQUADSYSTEM_API USquadEvaluator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFormationPreference FormationPreference = EFormationPreference::Balanced;
	
public:
	USquadMember* FindBestLeader(const TArray<USquadMember*>& Members) const;

	FHypotheticalFormation FindBestFormation(const TArray<USquadMember*>& Members, const int32 MaxMembersPerSquad);

protected:
	UFUNCTION(BlueprintNativeEvent)
	float PostLeaderEvaluation(const float InitialScore, USquadMember* Leader, const TArray<USquadMember*>& Members) const;
	
	UFUNCTION(BlueprintNativeEvent)
	float PostSquadEvaluation(const float InitialScore, const TArray<USquadMember*>& Members) const;

private:
	FHypotheticalFormation FindFewestSquadsFormation(const TArray<FHypotheticalFormation>& Formations);

	FHypotheticalFormation FindMostSquadsFormation(const TArray<FHypotheticalFormation>& Formations);

	FHypotheticalFormation FindBalancedFormation(const TArray<FHypotheticalFormation>& Formations);

	FHypotheticalFormation GetRandomFormation(const TArray<FHypotheticalFormation>& Formations);

	TArray<FHypotheticalFormation> FindBestFormations(const TArray<FHypotheticalSquad>& Squads, const int32 MemberCount);

	TArray<FHypotheticalSquad> GenerateHypotheticalSquads(const TArray<USquadMember*>& Members, const int32 MaxSquadSize) const;

	float EvaluateSquad(FHypotheticalSquad& Squad) const;
};
