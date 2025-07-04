// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory.h"
#include "Base/InventoriesManager.h"
#include "TetrisInventoriesManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UTetrisInventoriesManager : public UInventoriesManager
{
	GENERATED_BODY()

public:
	UTetrisInventoriesManager();
	
	UFUNCTION(BlueprintCallable)
	bool TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory);
	
	UFUNCTION(BlueprintCallable)
	bool TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition);
	
	UFUNCTION(BlueprintCallable)
	bool TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB);
};
