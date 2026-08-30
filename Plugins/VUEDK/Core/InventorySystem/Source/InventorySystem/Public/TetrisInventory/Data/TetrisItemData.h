// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/ItemDataBase.h"
#include "TetrisInventory/TetrisItem.h"
#include "TetrisItemData.generated.h"

UCLASS(NotBlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UTetrisItemData : public UItemDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	FIntPoint Size;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeRotated;

public:
	UTetrisItemData(): Size(FIntPoint(1, 1)),
	                   bCanBeRotated(false)
	{
		ItemClass = UTetrisItem::StaticClass();
	}
};
