// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/ItemDataBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryRegistrySubsystem.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UInventoryRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FGuid, UItemDataBase*> RegisteredItems;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	bool IsItemInRegistry(const UItemDataBase* ItemData) const;
	
	UItemDataBase* GetItemDataFromRegistry(const FGuid& ItemDataId) const;
};
