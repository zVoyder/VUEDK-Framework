// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BloodDecalSpawnParams.generated.h"

USTRUCT(BlueprintType)
struct FBloodDecalSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, ClampMax = 1.f))
	float SpawnChance;
	UPROPERTY(EditDefaultsOnly)
	float MaxSize;
	UPROPERTY(EditDefaultsOnly)
	float MinSize;
	UPROPERTY(EditDefaultsOnly)
	int MaxSortOrder;
	UPROPERTY(EditDefaultsOnly)
	int MinSortOrder;

	FBloodDecalSpawnParams(): SpawnChance(0.8f),
	                          MaxSize(50.f),
	                          MinSize(15.f),
	                          MaxSortOrder(10),
	                          MinSortOrder(-10)
	{
	}
};
