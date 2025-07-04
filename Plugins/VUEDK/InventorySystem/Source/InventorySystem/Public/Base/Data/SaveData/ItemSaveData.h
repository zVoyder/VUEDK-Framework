// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EquipSlotIndex;

	FItemSaveData(): Quantity(0),
	                 EquipSlotIndex(-1)
	{
	}
};
