// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentGoal.h"
#include "GOAPGoalCreationData.generated.h"

USTRUCT(BlueprintType)
struct FGOAPGoalCreationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAgentGoal> Goal;
	UPROPERTY(EditDefaultsOnly)
	TArray<FGOAPBeliefEntryData> DesiredEffects;

	FGOAPGoalCreationData() : Goal(nullptr),
	                          DesiredEffects()
	{
	}
};
