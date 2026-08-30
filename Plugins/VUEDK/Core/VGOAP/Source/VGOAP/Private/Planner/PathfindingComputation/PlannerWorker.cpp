// Copyright VUEDK, Inc. All Rights Reserved.

#include "Planner/PathfindingComputation/PlannerWorker.h"

TArray<FActionSnapshot> FPlannerWorker::FindAgentPath(const TArray<FActionSnapshot> Actions, TArray<FGoalSnapshot> Goals, const float PriorityLevel, const FGoalSnapshot& MostRecentGoal, FGoalSnapshot& OutSelectedGoal, float& OutTotalCost)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPlannerWorker::FindAgentPath);
	FilterGoals(Goals, PriorityLevel);
	SortGoalsByPriority(Goals, MostRecentGoal);

	for (FGoalSnapshot& Goal : Goals)
	{
		FNode GoalNode = FNode(
			nullptr,
			FActionSnapshot(),
			Goal.DesiredEffects,
			0.0f,
			0.0f
		);
		
		OutSelectedGoal = Goal;
		TArray<FActionSnapshot> ActionStack;
		if (ComputePath(GoalNode, Actions, ActionStack, OutTotalCost))
			return ActionStack;
	}

	return TArray<FActionSnapshot>();
}

bool FPlannerWorker::ComputePath(const FNode& GoalNode, const TArray<FActionSnapshot>& AllActions, TArray<FActionSnapshot>& OutActionStack, float& OutTotalCost)
{
	FMemMark Mark(FMemStack::Get());
	
	TArray<FNode*> OpenSet;
	FNode* StartNode = new(FMemStack::Get()) FNode(GoalNode);
	OpenSet.HeapPush(StartNode, FNodeMinHeapComparator());

	// Backward A*
	while (OpenSet.Num() > 0)
	{
		FNode* CurrentNode = nullptr;
		OpenSet.HeapPop(CurrentNode, FNodeMinHeapComparator());

		if (!CurrentNode)
			continue;

		TSet<FBeliefSnapshot> CurrentRequiredEffects(CurrentNode->RequiredEffects);
		FilterEffectsByState(CurrentRequiredEffects);
		if (CurrentRequiredEffects.IsEmpty())
		{
			if (!CurrentNode || CurrentNode->IsLeafDead())
				return false;

			// Path found
			OutTotalCost = CurrentNode->Cost;
			OutActionStack = ConstructActionStack(CurrentNode, AllActions);
			return OutActionStack.Num() > 0;
		}

		for (const FActionSnapshot& Action : AllActions)
		{
			if (!IsActionUsefulForEffects(Action, CurrentRequiredEffects))
				continue;

			TSet<FBeliefSnapshot> NewRequiredEffects = BuildNextRequiredEffects(Action, CurrentRequiredEffects);
			const float NewCost = CurrentNode->Cost + Action.Cost;
			const float NewHeuristic = NewRequiredEffects.Num();
			
			FNode* NewNode = new(FMemStack::Get()) FNode(
				CurrentNode,
				Action,
				NewRequiredEffects,
				NewCost,
				NewHeuristic
			);

			OpenSet.HeapPush(NewNode, FNodeMinHeapComparator());
			CurrentNode->Leaves.Add(NewNode);
		}
	}
	
	return false;
}

TArray<FActionSnapshot> FPlannerWorker::ConstructActionStack(FNode*& FinalNode, const TArray<FActionSnapshot>& AllActions)
{
	TArray<FActionSnapshot> ActionStack;
	ActionStack.Heapify(FStackComparator());

	for (const FNode* Node = FinalNode; Node; Node = Node->Parent)
		ActionStack.HeapPush(Node->Action, FStackComparator());

	ActionStack.HeapSort(FStackComparator());
	return ActionStack;
}

void FPlannerWorker::FilterGoals(TArray<FGoalSnapshot>& InGoals, const float PriorityLevel)
{
	TArray<FGoalSnapshot> FilteredGoals;
	for (const FGoalSnapshot& Goal : InGoals)
	{
		if (Goal.bIsPursuable && Goal.Priority >= PriorityLevel) // Consider only pursuable goals with sufficient priority
			FilteredGoals.Add(Goal);
	}

	InGoals = FilteredGoals;
}

void FPlannerWorker::SortGoalsByPriority(TArray<FGoalSnapshot>& InGoals, FGoalSnapshot InMostRecentGoal)
{
	InGoals.Sort([InMostRecentGoal](const FGoalSnapshot& A, const FGoalSnapshot& B)
	{
		const float PriorityA = (A == InMostRecentGoal) ? A.Priority - 0.01f : A.Priority;
		const float PriorityB = (B == InMostRecentGoal) ? B.Priority - 0.01f : B.Priority;
		return PriorityA > PriorityB;
	});
}

void FPlannerWorker::FilterEffectsByState(TSet<FBeliefSnapshot>& Effects)
{
	for (auto It = Effects.CreateIterator(); It; ++It)
	{
		if (It->bEvaluatedValue)
			It.RemoveCurrent();
	}
}

bool FPlannerWorker::IsActionUsefulForEffects(const FActionSnapshot& Action, const TSet<FBeliefSnapshot>& RequiredEffects)
{
	for (const FBeliefSnapshot& Effect : Action.EvaluatedEffects)
	{
		if (RequiredEffects.Contains(Effect))
			return true;
	}

	return false;
}

TSet<FBeliefSnapshot> FPlannerWorker::BuildNextRequiredEffects(const FActionSnapshot& Action, const TSet<FBeliefSnapshot>& RequiredEffects)
{
	TSet<FBeliefSnapshot> Result(RequiredEffects);

	for (const FBeliefSnapshot& Effect : Action.EvaluatedEffects)
		Result.Remove(Effect);

	for (const FBeliefSnapshot& Precondition : Action.EvaluatedPreconditions)
		Result.Add(Precondition);

	return Result;
}
