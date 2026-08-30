// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Agent/AgentBelief.h"
#include "GOAPBeliefData.generated.h"

USTRUCT(BlueprintType)
struct FGOAPBeliefEntryData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag BeliefTag;
	UPROPERTY(EditDefaultsOnly)
	bool bValue = true;
};

USTRUCT(BlueprintType)
struct FGOAPBeliefCondition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAgentBelief> Belief = nullptr;
	UPROPERTY(EditDefaultsOnly)
	bool bValue = true;

	FORCEINLINE bool operator==(const FGOAPBeliefCondition& Other) const
	{
		return Belief == Other.Belief && bValue == Other.bValue;
	}

	FORCEINLINE bool Evaluate() const
	{
		if (!IsValid(Belief))
			return false;

		return Belief->Evaluate() == bValue;
	}
	
	FORCEINLINE bool CheckEvaluation() const
	{
		if (!IsValid(Belief))
			return false;

		return Belief->GetLastState() == bValue;
	}
};
