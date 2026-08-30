// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAgent.h"
#include "UObject/Object.h"
#include "AgentContext.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType)
class VGOAP_API UAgentContext : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UGOAPAgent> Agent;
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;
	UPROPERTY()
	TObjectPtr<AAIController> OwnerController;
	UPROPERTY()
	TObjectPtr<UGOAPBlackboard> Blackboard;
	UPROPERTY()
	TObjectPtr<UGOAPBlackboard> GlobalBlackboard;

public:
	void Init(UGOAPAgent* InAgent, UGOAPBlackboard* InBlackboard, UGOAPBlackboard* InGlobalBlackboard);

	void SetupContext(const UGOAPAgent* InAgent);

	void PawnChanged(APawn* Pawn);
};
