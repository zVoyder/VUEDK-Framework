// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveData/TetrisInventory/TetrisInventorySaveData.h"
#include "RPGInventorySaveData.generated.h"

UCLASS()
class RPGSAVEBRIDGE_API URPGInventorySaveData : public UTetrisInventorySaveData
{
	GENERATED_BODY()

public:
	virtual void RegisterItemsNative() override;
};
