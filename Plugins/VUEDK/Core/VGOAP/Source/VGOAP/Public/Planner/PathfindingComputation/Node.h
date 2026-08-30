// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "ActionSnapshot.h"

struct FNode
{
	const FNode* Parent;
	FActionSnapshot Action;
	TSet<FBeliefSnapshot> RequiredEffects;
	TSet<FNode*> Leaves;
	float Cost;
	float Heuristic;

	FNode() : Parent(nullptr),
	          Cost(0.0f),
	          Heuristic(0.0f)
	{
	}

	FNode(const FNode* InParent, const FActionSnapshot& InAction, const TSet<FBeliefSnapshot>& InRequiredEffects, const float InCost, const float InHeuristic) : Parent(InParent),
	                                                                                                                                                             Action(InAction),
	                                                                                                                                                             RequiredEffects(InRequiredEffects),
	                                                                                                                                                             Leaves(),
	                                                                                                                                                             Cost(InCost),
	                                                                                                                                                             Heuristic(InHeuristic)
	{
	}

	FORCEINLINE bool IsLeafDead() const
	{
		return Leaves.Num() == 0 && !Action.IsValid();
	}

	FORCEINLINE bool operator==(const FNode& Other) const
	{
		return RequiredEffects.Num() == Other.RequiredEffects.Num() &&
			RequiredEffects.Includes(Other.RequiredEffects);
	}
};

FORCEINLINE uint32 GetTypeHash(const FNode& Node)
{
	uint32 Hash = 0;
	
	for (const FBeliefSnapshot& Belief : Node.RequiredEffects)
		Hash = HashCombine(Hash, GetTypeHash(Belief.BeliefTag));

	Hash = HashCombine(Hash, GetTypeHash(Node.Action));
	Hash = HashCombine(Hash, GetTypeHash(Node.Cost));
	Hash = HashCombine(Hash, GetTypeHash(Node.Heuristic));
	return Hash;
}
