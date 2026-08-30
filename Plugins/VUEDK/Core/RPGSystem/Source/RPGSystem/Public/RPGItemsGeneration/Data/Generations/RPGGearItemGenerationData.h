// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/RPGItemBaseGenerationData.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "RPGItemsGeneration/RPGItemStatOperation.h"
#include "RPGGearItemGenerationData.generated.h"

USTRUCT(BlueprintType)
struct RPGSYSTEM_API FStatOperationWithProbability
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	URPGItemStatOperation* StatOperation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Probability;

	FStatOperationWithProbability()
		: StatOperation(nullptr), Probability(100.0f)
	{
	}
};

UCLASS()
class RPGSYSTEM_API URPGGearItemGenerationData : public URPGItemBaseGenerationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TMap<UCoreStatData*, URPGItemStatOperation*> StatsModifiers;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<UCoreStatData*, FStatOperationWithProbability> AdditionalStatsModifiers;
};
