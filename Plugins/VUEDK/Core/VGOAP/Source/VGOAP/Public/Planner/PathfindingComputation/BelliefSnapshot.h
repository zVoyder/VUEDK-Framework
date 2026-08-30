// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Data/GOAPBeliefData.h"

struct FBeliefSnapshot
{
	FGameplayTag BeliefTag;
	bool bEvaluatedValue;

	FBeliefSnapshot() : bEvaluatedValue(false)
	{
	}

	explicit FBeliefSnapshot(const FGOAPBeliefCondition& InBeliefData)
	{
		BeliefTag = InBeliefData.Belief->GetTag();
		bEvaluatedValue = InBeliefData.Evaluate();
	}

	FORCEINLINE bool operator==(const FBeliefSnapshot& Other) const
	{
		return BeliefTag == Other.BeliefTag;
	}
};

FORCEINLINE uint32 GetTypeHash(const FBeliefSnapshot& BeliefSnapshot)
{
	return GetTypeHash(BeliefSnapshot.BeliefTag);
}
