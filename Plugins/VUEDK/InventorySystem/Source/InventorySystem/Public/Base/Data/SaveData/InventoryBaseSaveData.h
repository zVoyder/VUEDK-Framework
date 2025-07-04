// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData.h"
#include "InventoryBaseSaveData.generated.h"

UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UInventoryBaseSaveData : public USaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double MaxWeight;
};
