// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/SaveData/InventoryBaseSaveData.h"
#include "ClassicInventory/Data/SaveData/ClassicItemSaveData.h"
#include "ClassicInventorySaveData.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UClassicInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FClassicItemSaveData> SavedItems;
};
