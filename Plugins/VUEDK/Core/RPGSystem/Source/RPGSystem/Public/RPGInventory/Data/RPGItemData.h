// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory/Data/TetrisItemData.h"
#include "RPGInventory/Items/RPGItem.h"
#include "RPGItemData.generated.h"

UCLASS()
class RPGSYSTEM_API URPGItemData : public UTetrisItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseRarity = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseRarity"))
	bool bUsePredefinedRarity = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseRarity && bUsePredefinedRarity"))
	URPGRarityLevelData* PredefinedRarityLevelData = nullptr;
	
public:
	URPGItemData()
	{
		ItemClass = URPGItem::StaticClass();
	}
};
