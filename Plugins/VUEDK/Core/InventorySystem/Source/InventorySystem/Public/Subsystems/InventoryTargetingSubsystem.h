// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/InventoryBase.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InventoryTargetingSubsystem.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UInventoryTargetingSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UInventoryBase> MainInventory;
	UPROPERTY(Transient)
	TWeakObjectPtr<UInventoryBase> TargetInventory;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetMainInventory(UInventoryBase* Inventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetTargetInventory(UInventoryBase* Inventory);

	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	UInventoryBase* GetMainInventory() const;

	UFUNCTION(BlueprintPure, Category = "Tetris Inventory System")
	UInventoryBase* GetTargetInventory() const;
};
