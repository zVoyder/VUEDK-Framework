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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|Gameplay|Inventory System")
	static UItemBase* CreateItem(UObject* WorldContextObject, UItemDataBase* Data);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|Gameplay|Inventory System")
	static UItemBase* CreateItems(UObject* WorldContextObject, UItemDataBase* Data, int32& OutOverflow, const int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "VUEDK|Gameplay|Inventory System")
	static AItemDropActor* CreateItemDropActor(const UObject* WorldContextObject, UItemBase* Item);
};
