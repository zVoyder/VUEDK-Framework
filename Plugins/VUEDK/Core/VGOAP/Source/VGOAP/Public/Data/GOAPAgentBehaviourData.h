// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GOAPActionCreationData.h"
#include "GOAPGoalCreationData.h"
#include "Agent/AgentSensor.h"
#include "Engine/DataAsset.h"
#include "GOAPAgentBehaviourData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VGOAP_API UGOAPAgentBehaviourData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, TObjectPtr<UAgentSensor>> Sensors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, TObjectPtr<UAgentBelief>> Beliefs;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGOAPActionCreationData> Actions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGOAPGoalCreationData> Goals;
};
