// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "BelliefSnapshot.h"
#include "GameplayTagContainer.h"
#include "Agent/AgentAction.h"

struct FActionSnapshot
{
	FGameplayTag ActionTag;
	float Priority;
	float Cost;
	TArray<FBeliefSnapshot> EvaluatedPreconditions;
	TArray<FBeliefSnapshot> EvaluatedEffects;

	FActionSnapshot() : Priority(0.f),
	                    Cost(0.f)
	{
	}

	explicit FActionSnapshot(const UAgentAction* InAction)
	{
		if (!InAction)
			return;

		ActionTag = InAction->GetTag();
		Priority = InAction->GetPriority();
		Cost = InAction->GetCost();

		for (const FGOAPBeliefCondition& Precondition : InAction->GetPreconditions())
		{
			FBeliefSnapshot BeliefSnapshot = FBeliefSnapshot(Precondition);
			EvaluatedPreconditions.Add(BeliefSnapshot);
		}

		for (const FGOAPBeliefCondition& Effect : InAction->GetEffects())
		{
			FBeliefSnapshot BeliefSnapshot = FBeliefSnapshot(Effect);
			EvaluatedEffects.Add(BeliefSnapshot);
		}
	}

	FORCEINLINE bool operator==(const FActionSnapshot& Action) const
	{
		return ActionTag == Action.ActionTag;
	}

	FORCEINLINE bool operator==(const UAgentAction* InAction) const
	{
		if (!InAction)
			return false;

		return ActionTag == InAction->GetTag();
	}

	FORCEINLINE bool IsValid() const
	{
		return ActionTag.IsValid();
	}
};

FORCEINLINE uint32 GetTypeHash(const FActionSnapshot& ActionSnapshot)
{
	return GetTypeHash(ActionSnapshot.ActionTag);
}
