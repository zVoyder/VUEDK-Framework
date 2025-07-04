// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/SaveData/ItemSaveData.h"
#include "TetrisItemSaveData.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FTetrisItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSaveData ItemSaveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint SlotPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRotated;

	FTetrisItemSaveData(): SlotPosition(),
	                       bIsRotated(false)
	{
	}
};
