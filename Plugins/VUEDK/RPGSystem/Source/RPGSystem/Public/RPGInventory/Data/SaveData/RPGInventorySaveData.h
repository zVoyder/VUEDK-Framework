// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGInventoryItemsSaveData.h"
#include "TetrisInventory/Data/SaveData/TetrisInventorySaveData.h"
#include "RPGInventorySaveData.generated.h"

UCLASS()
class RPGSYSTEM_API URPGInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FRPGInventoryItemsSaveData RPGItemsSaveData;
};
