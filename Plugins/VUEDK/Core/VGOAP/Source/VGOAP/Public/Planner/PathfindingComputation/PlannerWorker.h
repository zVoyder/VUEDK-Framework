// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "ActionSnapshot.h"
#include "GoalSnapshot.h"
#include "Node.h"

struct FNodeMinHeapComparator
{
	FORCEINLINE bool operator()(const FNode& A, const FNode& B) const
	{
		const float ScoreA = A.Cost + A.Heuristic;
		const float ScoreB = B.Cost + B.Heuristic;
		
		return ScoreA < ScoreB;
	}
};

struct FStackComparator
{
	FORCEINLINE bool operator()(const FActionSnapshot& A, const FActionSnapshot& B) const
	{
		if (FMath::Abs(A.Priority - B.Priority) < KINDA_SMALL_NUMBER)
			return true;

		return A.Priority < B.Priority;
	}
};

struct FPlannerWorker
{
	static TArray<FActionSnapshot> FindAgentPath(const TArray<FActionSnapshot> Actions, TArray<FGoalSnapshot> Goals, const float PriorityLevel, const FGoalSnapshot& MostRecentGoal, FGoalSnapshot& OutSelectedGoal, float& OutTotalCost);

private:
	static bool ComputePath(const FNode& GoalNode, const TArray<FActionSnapshot>& AllActions, TArray<FActionSnapshot>& OutActionStack, float& OutTotalCost);
	
	static TArray<FActionSnapshot> ConstructActionStack(FNode*& FinalNode, const TArray<FActionSnapshot>& AllActions);
	
	static void FilterGoals(TArray<FGoalSnapshot>& InGoals, const float PriorityLevel);

	static void SortGoalsByPriority(TArray<FGoalSnapshot>& InGoals, FGoalSnapshot InMostRecentGoal);

	static void FilterEffectsByState(TSet<FBeliefSnapshot>& Effects);

	static bool IsActionUsefulForEffects(const FActionSnapshot& Action, const TSet<FBeliefSnapshot>& RequiredEffects);

	static TSet<FBeliefSnapshot> BuildNextRequiredEffects(const FActionSnapshot& Action, const TSet<FBeliefSnapshot>& RequiredEffects);
};
