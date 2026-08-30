// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentAction.h"
#include "Agent/AgentGoal.h"
#include "UObject/Object.h"
#include "Plan.generated.h"

USTRUCT(NotBlueprintType)
struct VGOAP_API FPlan
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAgentGoal> AgentGoal;
	UPROPERTY()
	TArray<UAgentAction*> Stack;
	UPROPERTY()
	TMap<FGameplayTag, UAgentAction*> ActionsMap;
	float TotalCost;

public:
	FPlan() : AgentGoal(nullptr),
	          Stack(),
	          ActionsMap(),
	          TotalCost(0.f)
	{
	}

	FPlan(UAgentGoal* InGoal, const TArray<UAgentAction*>& InStack, const float InTotalCost) : AgentGoal(InGoal),
	                                                                                           Stack(InStack),
	                                                                                           TotalCost(InTotalCost)
	{
		for (UAgentAction* Action : InStack)
		{
			if (IsValid(Action))
				ActionsMap.Add(Action->GetTag(), Action);
		}
	}

	/**
	 * Clears the plan by resetting the goal, emptying the action stack and map, and resetting the total cost.
	 */
	FORCEINLINE void Clear()
	{
		AgentGoal = nullptr;
		Stack.Empty();
		ActionsMap.Empty();
		TotalCost = 0.f;
	}

	/**
	 * Advances the plan by popping the next action from the action list.
	 * @return The next UAgentAction in the plan.
	 */
	FORCEINLINE UAgentAction* Advance()
	{
		return Stack.Pop();
	}

	/**
	 * Peeks at the next action in the plan without removing it.
	 * @return The next UAgentAction in the plan.
	 */
	FORCEINLINE UAgentAction* Peek() const
	{
		return Stack.Num() > 0 ? Stack.Last() : nullptr;
	}

	/**
	 * Checks if there are no more actions left in the plan.
	 * @return True if the action list is empty, false otherwise.
	 */
	FORCEINLINE bool IsCompleted() const
	{
		return Stack.IsEmpty();
	}

	FORCEINLINE bool IsValidPlan() const
	{
		return IsValid(AgentGoal);
	}
};
