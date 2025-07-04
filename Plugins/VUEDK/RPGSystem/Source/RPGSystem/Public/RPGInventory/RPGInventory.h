// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/RPGInventoryItemsSaveData.h"
#include "TetrisInventory/TetrisInventory.h"
#include "RPGInventory.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGSYSTEM_API URPGInventory : public UTetrisInventory
{
	GENERATED_BODY()
	
public:
	URPGInventory();

protected:
	virtual USaveData* CreateSaveDataObject_Implementation() override;

	virtual USaveData* CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave) override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	void CreateRPGInventorySaveData(FRPGInventoryItemsSaveData& RPGItemsSaveData, TArray<UItemBase*>& ItemsToSave);

	void LoadRPGInventorySaveData(FRPGInventoryItemsSaveData& RPGItemsSaveData);
};
