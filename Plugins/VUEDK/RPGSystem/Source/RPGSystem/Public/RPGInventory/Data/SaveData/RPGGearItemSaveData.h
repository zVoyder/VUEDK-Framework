// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemSaveData.h"
#include "RPGGearItemSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRPGGearItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FRPGItemSaveData RPGItemSaveData;
	UPROPERTY(BlueprintReadWrite)
	TMap<FGuid, float> GearStats;

	FRPGGearItemSaveData(): RPGItemSaveData()
	{
	}
};
