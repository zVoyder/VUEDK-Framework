// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/RPGItemBaseGenerationData.h"
#include "RPGItemsGeneration/Data/FixedStatData.h"
#include "RPGGearItemFixedStatsGenerationData.generated.h"

UCLASS()
class RPGSYSTEM_API URPGGearItemFixedStatsGenerationData : public URPGItemBaseGenerationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FFixedStat> Stats;
};
