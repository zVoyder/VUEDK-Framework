// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory.h"
#include "Data/SaveData/TetrisItemSaveData.h"
#include "Base/ItemBase.h"
#include "TetrisItem.generated.h"

class UTetrisItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemRotated,
	FIntPoint, RelativeSize
	);

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UTetrisItem : public UItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnItemRotated OnItemRotated;
	
protected:
	// Current position of the item's top-left corner in the inventory grid
	UPROPERTY()
	FIntPoint TopLeftCornerPosition;
	UPROPERTY()
	bool bIsRotated;
	UPROPERTY()
	FIntPoint CachedSize;
	UPROPERTY()
	bool bCachedRotation;

public:
	UTetrisItem();

	virtual void Init(UObject* WorldContextObject, UItemDataBase* Data) override;

	FTetrisItemSaveData CreateTetrisSaveData() const;

	void LoadTetrisSaveData(UInventoryBase* LoadingInventory, const FTetrisItemSaveData& TetrisSaveData);

	UFUNCTION(BlueprintPure)
	bool CanRotate() const;

	UFUNCTION(BlueprintCallable)
	void SetRotation(const bool bNewRotation);
	
	UFUNCTION(BlueprintCallable)
	void ResetToCachedRotation();

	UFUNCTION(BlueprintCallable)
	void Rotate();

	UFUNCTION(BlueprintPure)
	UTetrisItemData* GetTetrisItemData() const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetCurrentPosition() const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetCachedSize() const;

	UFUNCTION(BlueprintPure)
	bool GetCachedRotation() const;
	
	UFUNCTION(BlueprintPure)
	FIntPoint GetRelativeSize() const;

	UFUNCTION(BlueprintPure)
	bool IsRotated() const;

	void SetCurrentPosition(const FIntPoint NewPosition);

	void CacheCurrentRotation();

protected:
	virtual void OnEquip_Implementation() override;
};
