// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "FixedStatData.generated.h"

USTRUCT(BlueprintType)
struct FFixedStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCoreStatData* StatData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Value;

	FFixedStat(): StatData(nullptr),
	              Value(0)
	{
	}
};
