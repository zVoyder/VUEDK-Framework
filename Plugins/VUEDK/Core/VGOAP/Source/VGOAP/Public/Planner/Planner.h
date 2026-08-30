// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "PathfindingComputation/ActionSnapshot.h"
#include "PathfindingComputation/GoalSnapshot.h"

class UGOAPAgent;

struct FPlanComputationResult
{
	TArray<FActionSnapshot> ActionStack;
	FGoalSnapshot SelectedGoal;
	float TotalCost = 0.f;
};

class VGOAP_API FPlanner
{
private:
	std::atomic<bool> bPlanning = false;

public:
	FPlanner() = default;
	~FPlanner() = default;

	bool RequestPlan(UGOAPAgent* InAgent, const UAgentGoal* InCurrentGoal, UAgentGoal* InMostRecentGoal, const bool bAsync);

private:
	static void ApplyPlan(FPlanner* PlannerInstance, const TWeakObjectPtr<UGOAPAgent> WeakAgent, const TSharedRef<FPlanComputationResult, ESPMode::ThreadSafe>& Result);
	
	static TArray<UAgentAction*> ReconstructActionStack(const UGOAPAgent* InAgent, const TArray<FActionSnapshot>& InActionSnapshots);

	static TArray<FActionSnapshot> ConvertActionsToSnapshots(const TMap<FGameplayTag, UAgentAction*>& InActions);

	static TArray<FGoalSnapshot> ConvertGoalsToSnapshots(const TMap<FGameplayTag, UAgentGoal*>& InGoals);
};
