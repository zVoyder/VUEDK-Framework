// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/ItemDataBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/InventoryRegistrySubsystem.h"
#include "ISInventoriesUtility.generated.h"

class UInventoryBase;

UCLASS()
class INVENTORYSYSTEM_API UISInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TWeakObjectPtr<UInventoryRegistrySubsystem> InventoryRegistrySubsystem;

public:
	static void Init(UInventoryRegistrySubsystem* InInventoryRegistrySubsystem);
	
	/**
	 * @brief Checks if the item data is registered in the items registry.
	 * @param ItemData The item data to check.
	 * @return True if the item data is registered, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory System")
	static bool IsItemInRegistry(const UItemDataBase* ItemData);

	/**
	 * @brief Checks if the item data is registered in the items registry.
	 * @param ItemDataId The Id of the item data to check.
	 * @return True if the item data is registered, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory System")
	static UItemDataBase* GetItemDataFromRegistry(const FGuid& ItemDataId);

	/**
	 * @brief Drops an item using the drop operation defined in the settings.
	 * @param Instigator The player controller who is dropping the item.
	 * @param Item The item to drop.
	 * @return True if the item was dropped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	static bool DropItemWithOperation(APlayerController* Instigator, UItemBase* Item);

private:
	static bool Check();
};
