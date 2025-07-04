// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "ItemsStatsData.generated.h"

UCLASS()
class RPGSYSTEM_API UItemsStatsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCoreStatData*> Stats;
};
