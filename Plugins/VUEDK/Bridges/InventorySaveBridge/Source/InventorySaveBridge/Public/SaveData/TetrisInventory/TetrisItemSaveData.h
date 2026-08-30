// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveData/Base/ItemBaseSaveData.h"
#include "TetrisItemSaveData.generated.h"

UCLASS()
class INVENTORYSAVEBRIDGE_API UTetrisItemSaveData : public UItemBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadOnly)
	FIntPoint SlotPosition;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	bool bIsRotated;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;
};
