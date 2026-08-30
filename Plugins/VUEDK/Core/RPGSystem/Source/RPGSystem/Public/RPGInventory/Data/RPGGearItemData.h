// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGGearItemData.generated.h"

UCLASS()
class RPGSYSTEM_API URPGGearItemData : public URPGItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GearBaseMultiplier = 1.0f;

public:
	URPGGearItemData()
	{
		ItemClass = URPGGearItem::StaticClass();
	}
};
