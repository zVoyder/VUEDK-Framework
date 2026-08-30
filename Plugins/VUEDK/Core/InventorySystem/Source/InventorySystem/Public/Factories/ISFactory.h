// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Drop/DropItemOperation.h"
#include "Base/Data/ItemDataBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISFactory.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UISFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a new item instance from the given item data.
	 * @param WorldContextObject The world context object.
	 * @param Data The item data to use for creation.
	 * @param OutOverflow Out parameter to hold the overflow amount if the item quantity exceeds the maximum stack size.
	 * @param Quantity The quantity to assign to the new item (default: 1).
	 * @return Pointer to the created UItemBase instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Gameplay|Inventory System")
	static UItemBase* CreateItem(const UObject* WorldContextObject, UItemDataBase* Data, int32& OutOverflow, const int32 Quantity = 1);

	static UItemBase* CreateSingleItem(const UObject* WorldContextObject, UItemDataBase* Data);
	
	/**
	 * Spawns an item drop actor in the world for the specified item.
	 * @param WorldContextObject The world context object.
	 * @param Item The item to associate with the drop actor.
	 * @return Pointer to the created AItemDropActor instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Gameplay|Inventory System")
	static AItemDropActor* CreateItemDropActor(const UObject* WorldContextObject, UItemBase* Item);

	/**
	 * Creates a new drop item operation instance.
	 * @param Instigator The player controller instigating the drop operation.
	 * @param DropOperationClass The class of the drop item operation to create.
	 * @return Pointer to the created UDropItemOperation instance.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay|Inventory System")
	static UDropItemOperation* CreateItemDropOperation(APlayerController* Instigator, const TSubclassOf<UDropItemOperation> DropOperationClass);
};
