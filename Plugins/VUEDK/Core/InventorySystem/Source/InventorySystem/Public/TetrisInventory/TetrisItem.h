// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory.h"
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

	virtual void Init(const TObjectPtr<UWorld> InWorld, UItemDataBase* Data) override;
	
	/**
	 * Checks if the item can be rotated based on its current data.
	 * @return True if the item can be rotated, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool CanRotate() const;

	/**
	 * Sets the rotation state of the item.
	 * @param bNewRotation The new rotation state to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRotation(const bool bNewRotation);

	/**
	 * Resets the item's rotation to its cached state.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetToCachedRotation();

	/**
	 * Rotates the item if it can be rotated.
	 */
	UFUNCTION(BlueprintCallable)
	void Rotate();

	/**
	 * Gets the Tetris item data associated with this item.
	 * @return The Tetris item data.
	 */
	UFUNCTION(BlueprintPure)
	UTetrisItemData* GetTetrisItemData() const;

	/**
	 * Gets the current position of the item's top-left corner in the inventory grid.
	 * @return The current position as an FIntPoint.
	 */
	UFUNCTION(BlueprintPure)
	FIntPoint GetCurrentPosition() const;

	/**
	 * Gets the cached size of the item.
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	FIntPoint GetCachedSize() const;

	/**
	 * Gets the cached rotation state of the item.
	 * @return True if the item is cached as rotated, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool GetCachedRotation() const;

	/**
	 * Gets the relative size of the item based on its current rotation state.
	 * @return The relative size as an FIntPoint.
	 */
	UFUNCTION(BlueprintPure)
	FIntPoint GetRelativeSize() const;

	/**
	 * Checks if the item is currently rotated.
	 * @return True if the item is rotated, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsRotated() const;

	/**
	 * Sets the current position of the item's top-left corner in the inventory grid.
	 * @param NewPosition The new position to set as an FIntPoint.
	 */
	void SetCurrentPosition(const FIntPoint NewPosition);

	/**
	 * Caches the current rotation state and size of the item.
	 */
	void CacheCurrentRotation();

protected:
	virtual void OnEquip_Implementation() override;
};
