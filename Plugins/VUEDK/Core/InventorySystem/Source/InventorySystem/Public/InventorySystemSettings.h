// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Drop/DropItemOperation.h"
#include "UObject/Object.h"
#include "InventorySystemSettings.generated.h"

UCLASS(Config = InventorySystemSettings, DefaultConfig, meta = (DisplayName = "Inventory System Settings"))
class INVENTORYSYSTEM_API UInventorySystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSoftClassPtr<UDropItemOperation> ItemsDropOperationClass = UDropItemOperation::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Items Registry")
	TSet<TSoftObjectPtr<UItemDataBase>> RegisteredItems;
};
