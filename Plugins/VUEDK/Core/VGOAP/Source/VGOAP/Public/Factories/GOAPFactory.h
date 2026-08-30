// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentSensor.h"
#include "Agent/GOAPBlackboard.h"
#include "Data/GOAPBlackboardData.h"
#include "Data/GOAPGoalCreationData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Planner/Plan.h"
#include "Planner/Planner.h"
#include "GOAPFactory.generated.h"

UCLASS()
class VGOAP_API UGOAPFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GOAP|Factory")
	static UAgentSensor* CreateAgentSensor(UGOAPAgent* InAgent, const TSubclassOf<UAgentSensor> InSensorClass);
	
	UFUNCTION(BlueprintCallable, Category = "GOAP|Factory")
	static UAgentBelief* CreateAgentBelief(UGOAPAgent* InAgent, const TSubclassOf<UAgentBelief> InBeliefClass);

	UFUNCTION(BlueprintCallable, Category = "GOAP|Factory")
	static UAgentAction* CreateAgentAction(UGOAPAgent* InAgent, const TSubclassOf<UAgentAction> InActionClass, const TArray<FGOAPBeliefEntryData>& InPreconditions, const TArray<FGOAPBeliefEntryData>& InEffects);

	UFUNCTION(BlueprintCallable, Category = "GOAP|Factory")
	static UAgentGoal* CreateAgentGoal(UGOAPAgent* InAgent, const TSubclassOf<UAgentGoal> InGoalClass, const TArray<FGOAPBeliefEntryData>& InDesiredEffects);

	UFUNCTION(BlueprintPure, Category = "GOAP|Factory")
	static UGOAPBlackboard* CreateBlackboard(UGOAPBlackboardData* InBlackboardData);

	static UAgentContext* CreateAgentContext(UGOAPAgent* InAgent);

	static UAgentSensor* CreateSensorWithTemplate(UGOAPAgent* InAgent, const UAgentSensor* InSensorTemplate);

	static UAgentBelief* CreateBeliefWithTemplate(UGOAPAgent* InAgent, const UAgentBelief* InBeliefTemplate);

	static UAgentAction* CreateActionWithTemplate(UGOAPAgent* InAgent, const FGOAPActionCreationData& InActionData);
	
	static UAgentGoal* CreateGoalWithTemplate(UGOAPAgent* InAgent, const FGOAPGoalCreationData& InGoalData);
};
