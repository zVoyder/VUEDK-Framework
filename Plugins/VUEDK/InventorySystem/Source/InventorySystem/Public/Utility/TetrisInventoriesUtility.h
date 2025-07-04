// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory/TetrisInventoriesManager.h"
#include "TetrisInventory/TetrisInventoryEventsHandlerSubsystem.h"
#include "TetrisInventory/TetrisItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TetrisInventoriesUtility.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UTetrisInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UTetrisInventoryEventsHandlerSubsystem* EventsHandlerSubsystem;
	
public:
	static void Init(UTetrisInventoryEventsHandlerSubsystem* InEventsHandlerSubsystem);
	
	UFUNCTION(BlueprintPure, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static UTetrisInventoriesManager* GetTetrisInventoriesManager();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool TryTransferItem(UTetrisItem* Item, UTetrisInventory* DestinationInventory);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool TryTransferItemAtPosition(UTetrisItem* Item, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool TryTransferSwitchItems(UTetrisItem* ItemA, UTetrisItem* ItemB);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestDrag();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestDrop();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestRotate();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestEquipUnequip(const int32 SwapPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestTransferItemToTargetInventory();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestDiscard();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestSplit();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static void RequestCancelSplit();

	UFUNCTION(BlueprintPure, Category = "VUEDK|Gameplay|Tetris Inventory System")
	static bool IsSplitting();

private:
	static bool Check();
};
