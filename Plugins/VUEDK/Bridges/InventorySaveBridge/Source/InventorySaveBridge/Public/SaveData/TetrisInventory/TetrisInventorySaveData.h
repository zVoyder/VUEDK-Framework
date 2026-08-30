// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveData/Base/InventoryBaseSaveData.h"
#include "TetrisInventorySaveData.generated.h"

UCLASS()
class INVENTORYSAVEBRIDGE_API UTetrisInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	virtual void RegisterItemsNative() override;
	
protected:
	virtual void PostLoadItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData) override;
};
