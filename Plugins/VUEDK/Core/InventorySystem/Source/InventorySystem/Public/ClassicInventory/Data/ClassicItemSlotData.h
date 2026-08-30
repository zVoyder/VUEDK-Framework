// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicItemSlotData.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FClassicItemSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 StackSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SlotQuantity;

 	FClassicItemSlotData(): StackSize(1),
	                        SlotQuantity(0)
	{
	}
	
	bool IsMaxStacked() const
	{
		return SlotQuantity >= StackSize;
	}
};