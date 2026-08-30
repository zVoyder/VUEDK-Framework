// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "BelliefSnapshot.h"
#include "Agent/AgentGoal.h"

struct FGoalSnapshot
{
	FGameplayTag GoalTag;
	TSet<FBeliefSnapshot> DesiredEffects;
	float Priority;
	bool bIsPursuable;

	FGoalSnapshot() : Priority(0.0f),
	                  bIsPursuable(false)
	{
	}

	explicit FGoalSnapshot(UAgentGoal* InGoal)
	{
		if (!InGoal)
			return;

		GoalTag = InGoal->GetTag();
		Priority = InGoal->GetPriority();
		bIsPursuable = InGoal->IsPursuable();

		for (const FGOAPBeliefCondition& DesiredEffect : InGoal->GetDesiredEffects())
		{
			FBeliefSnapshot BeliefSnapshot = FBeliefSnapshot(DesiredEffect);
			DesiredEffects.Add(BeliefSnapshot);
		}
	}

	FORCEINLINE bool operator==(const FGoalSnapshot& GoalSnapshot) const
	{
		return GoalTag == GoalSnapshot.GoalTag;
	}
};

FORCEINLINE uint32 GetTypeHash(const FGoalSnapshot& GoalSnapshot)
{
	return GetTypeHash(GoalSnapshot.GoalTag);
}
