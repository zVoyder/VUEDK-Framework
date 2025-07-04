// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/ItemDataBase.h"
#include "Base/ItemDropActor.h"
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
	 * @return Pointer to the created UItemBase instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|Gameplay|Inventory System")
	static UItemBase* CreateItem(UObject* WorldContextObject, UItemDataBase* Data);

	/**
	 * Creates multiple item instances from the given item data and quantity.
	 * @param WorldContextObject The world context object.
	 * @param Data The item data to use for creation.
	 * @param OutOverflow The number of items that could not be created due to overflow.
	 * @param Quantity The number of items to create (default: 1).
	 * @return Pointer to the created UItemBase instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|Gameplay|Inventory System")
	static UItemBase* CreateItems(UObject* WorldContextObject, UItemDataBase* Data, int32& OutOverflow, const int32 Quantity = 1);

	/**
	 * Spawns an item drop actor in the world for the specified item.
	 * @param WorldContextObject The world context object.
	 * @param Item The item to associate with the drop actor.
	 * @return Pointer to the created AItemDropActor instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|Gameplay|Inventory System")
	static AItemDropActor* CreateItemDropActor(const UObject* WorldContextObject, UItemBase* Item);
};
