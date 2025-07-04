// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemsRegistry.h"
#include "InventoriesManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoriesManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UItemsRegistry* Registry;

public:
	UInventoriesManager();

	UFUNCTION(BlueprintPure)
	bool IsItemInRegistry(const UItemDataBase* ItemData) const;

	UFUNCTION(BlueprintCallable)
	UItemDataBase* GetItemDataFromRegistry(const FGuid& ItemDataID) const;

protected:
	virtual void BeginPlay() override;

private:
	bool Check() const;
};
