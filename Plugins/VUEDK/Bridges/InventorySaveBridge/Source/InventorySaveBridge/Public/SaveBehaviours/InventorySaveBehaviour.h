// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/InventoryBase.h"
#include "Components/Savers/Behaviours/SaveBehaviour.h"
#include "SaveData/Base/InventoryBaseSaveData.h"
#include "InventorySaveBehaviour.generated.h"

UCLASS()
class INVENTORYSAVEBRIDGE_API UInventorySaveBehaviour : public USaveBehaviour
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UInventoryBase* Inventory;
	
public:
	virtual void BeginPlay() override;

	virtual USaveDataBase* CreateSaveDataInstance_Implementation() override;

	virtual bool Save_Implementation(USaveDataBase* SaveData) override;

	virtual bool Load_Implementation(USaveDataBase* SaveData) override;
	
	virtual bool Check() const override;
};
