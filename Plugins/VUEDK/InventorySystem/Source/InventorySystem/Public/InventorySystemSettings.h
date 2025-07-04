// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/DropItemOperation.h"
#include "UObject/Object.h"
#include "InventorySystemSettings.generated.h"

UCLASS(Config = InventorySystemSettings, DefaultConfig, meta = (DisplayName = "Inventory System Settings"))
class INVENTORYSYSTEM_API UInventorySystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSoftClassPtr<UDropItemOperation> ItemsDropOperationClass = UDropItemOperation::StaticClass();
};
