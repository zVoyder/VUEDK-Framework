// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentAction.h"
#include "GOAPActionCreationData.generated.h"

USTRUCT(BlueprintType)
struct FGOAPActionCreationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAgentAction> Action;
	UPROPERTY(EditDefaultsOnly)
	TArray<FGOAPBeliefEntryData> Preconditions;
	UPROPERTY(EditDefaultsOnly)
	TArray<FGOAPBeliefEntryData> Effects;

	FGOAPActionCreationData() : Action(nullptr),
	                            Preconditions(),
	                            Effects()
	{
	}
};
