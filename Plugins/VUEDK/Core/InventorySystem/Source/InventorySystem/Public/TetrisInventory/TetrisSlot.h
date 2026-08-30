// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TetrisSlot.generated.h"

class UTetrisItem;
class UTetrisInventory;

UCLASS(Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API UTetrisSlot : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTetrisInventory* RelatedInventory;
	
private:
	FIntPoint SlotPosition;
	bool bIsOccupied;
	UPROPERTY()
	UTetrisItem* Item;

public:
	void Init(UTetrisInventory* InInventory, const FIntPoint InSlotPosition);
	
	UFUNCTION(BlueprintCallable)
	void OccupySlot(UTetrisItem* InItem);

	UFUNCTION(BlueprintCallable)
	void FreeSlot();

	UFUNCTION(BlueprintPure)
	bool IsOccupied() const;
	
	UFUNCTION(BlueprintPure)
	UTetrisItem* GetSlotItem() const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetSlotPosition() const;
};
