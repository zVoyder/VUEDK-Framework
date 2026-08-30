// Copyright VUEDK, Inc. All Rights Reserved.

#include "Planner/Planner.h"
#include "Agent/GOAPAgent.h"
#include "Factories/GOAPFactory.h"
#include "Planner/PathfindingComputation/PlannerWorker.h"

bool FPlanner::RequestPlan(UGOAPAgent* InAgent, const UAgentGoal* InCurrentGoal, UAgentGoal* InMostRecentGoal, const bool bAsync)
{
	if (!IsValid(InAgent))
		return false;

	if (bPlanning.exchange(true))
		return false;
	
	FPlanner* PlannerInstance = this;
	TWeakObjectPtr<UGOAPAgent> WeakAgent = InAgent;
	TArray<FActionSnapshot> ActionsSnapshots = ConvertActionsToSnapshots(InAgent->GetActions());
	TArray<FGoalSnapshot> GoalsSnapshots = ConvertGoalsToSnapshots(InAgent->GetGoals());
	const FGoalSnapshot MostRecentGoal = FGoalSnapshot(InMostRecentGoal);
	float PriorityLevel = IsValid(InCurrentGoal) ? InCurrentGoal->GetDynamicPriority() : 0.0f;
	TSharedRef<FPlanComputationResult, ESPMode::ThreadSafe> Result = MakeShared<FPlanComputationResult, ESPMode::ThreadSafe>();

	if (bAsync)
	{
		Async(EAsyncExecution::ThreadPool, [PlannerInstance, WeakAgent, ActionsSnapshots = MoveTemp(ActionsSnapshots), GoalsSnapshots = MoveTemp(GoalsSnapshots), MostRecentGoal, PriorityLevel, Result]()
		{
			Result->ActionStack = FPlannerWorker::FindAgentPath(ActionsSnapshots, GoalsSnapshots, PriorityLevel, MostRecentGoal, Result->SelectedGoal, Result->TotalCost);

			AsyncTask(ENamedThreads::GameThread, [PlannerInstance, WeakAgent, Result]()
			{
				ApplyPlan(PlannerInstance, WeakAgent, Result);
			});
		});

		return true;
	}

	Result->ActionStack = FPlannerWorker::FindAgentPath(ActionsSnapshots, GoalsSnapshots, PriorityLevel, MostRecentGoal, Result->SelectedGoal, Result->TotalCost);
	ApplyPlan(PlannerInstance, WeakAgent, Result);
	return true;
}

void FPlanner::ApplyPlan(FPlanner* PlannerInstance, const TWeakObjectPtr<UGOAPAgent> WeakAgent, const TSharedRef<FPlanComputationResult, ESPMode::ThreadSafe>& Result)
{
	if (!PlannerInstance)
		return;

	PlannerInstance->bPlanning.store(false, std::memory_order_release);
	
	if (!WeakAgent.IsValid())
		return;

	UGOAPAgent* Agent = WeakAgent.Get();
	const TArray<UAgentAction*> AgentActions = PlannerInstance->ReconstructActionStack(Agent, Result->ActionStack);
	UAgentGoal* Goal = Agent->FindGoal(Result->SelectedGoal.GoalTag);
	const FPlan Plan(Goal, AgentActions, Result->TotalCost);
	Agent->ApplyRequestedPlan(Plan);
}

TArray<UAgentAction*> FPlanner::ReconstructActionStack(const UGOAPAgent* InAgent, const TArray<FActionSnapshot>& InActionSnapshots)
{
	TArray<UAgentAction*> ActionStack;
	for (const FActionSnapshot& ActionSnapshot : InActionSnapshots)
	{
		UAgentAction* Action = InAgent->FindAction(ActionSnapshot.ActionTag);
		if (!IsValid(Action))
			continue;

		ActionStack.Add(Action);
	}

	return ActionStack;
}

TArray<FActionSnapshot> FPlanner::ConvertActionsToSnapshots(const TMap<FGameplayTag, UAgentAction*>& InActions)
{
	TArray<FActionSnapshot> Actions;

	for (const auto& ActionPair : InActions)
	{
		UAgentAction* Action = ActionPair.Value;
		if (!IsValid(Action))
			continue;

		FActionSnapshot ActionSnapshot = FActionSnapshot(Action);
		Actions.Add(ActionSnapshot);
	}
	
	return Actions;
}

TArray<FGoalSnapshot> FPlanner::ConvertGoalsToSnapshots(const TMap<FGameplayTag, UAgentGoal*>& InGoals)
{
	TArray<FGoalSnapshot> Goals;

	for (const auto& GoalPair : InGoals)
	{
		UAgentGoal* Goal = GoalPair.Value;
		if (!IsValid(Goal))
			continue;

		FGoalSnapshot GoalSnapshot = FGoalSnapshot(Goal);
		Goals.Add(GoalSnapshot);
	}

	return Goals;
}
