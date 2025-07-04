// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Data/ItemDataBase.h"
#include "ClassicInventory/ClassicItem.h"
#include "UObject/Object.h"
#include "ClassicItemData.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class INVENTORYSYSTEM_API UClassicItemData : public UItemDataBase
{
	GENERATED_BODY()

public:
	UClassicItemData()
	{
		ItemClass = UClassicItem::StaticClass();
	}
};
