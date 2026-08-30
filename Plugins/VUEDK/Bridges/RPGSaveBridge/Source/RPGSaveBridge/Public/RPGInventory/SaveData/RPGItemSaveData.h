// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemsGeneration/Data/Visuals/RPGItemVisualDetails.h"
#include "SaveData/TetrisInventory/TetrisItemSaveData.h"
#include "RPGItemSaveData.generated.h"

UCLASS()
class RPGSAVEBRIDGE_API URPGItemSaveData : public UTetrisItemSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FGuid SavedRarityID;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FRPGItemVisualDetails SavedVisualDetails;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
