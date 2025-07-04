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

	/**
	 * Checks if the given item is present in the registry.
	 * @param ItemData The item data to check.
	 * @return True if the item is in the registry, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsItemInRegistry(const UItemDataBase* ItemData) const;

	/**
	 * Retrieves the item data from the registry by its unique ID.
	 * @param ItemDataID The unique identifier of the item data.
	 * @return Pointer to the UItemDataBase if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	UItemDataBase* GetItemDataFromRegistry(const FGuid& ItemDataID) const;

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * Checks the validity or state of the inventories manager.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;
};
