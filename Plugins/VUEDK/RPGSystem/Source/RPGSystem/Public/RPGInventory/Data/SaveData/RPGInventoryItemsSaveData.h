// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveArrays/RPGGearItemsSaveArray.h"
#include "SaveArrays/RPGItemsSaveArray.h"
#include "RPGInventoryItemsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRPGInventoryItemsSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FRPGItemsSaveArray> GenericItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FRPGGearItemsSaveArray> GearItems;
};
