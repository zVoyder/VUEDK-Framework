// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TetrisInventoryEventsHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestDrag
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestDrop
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestRotate
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnRequestEquipUnequip,
	int32, SwapPriority
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnRequestDiscard,
	APlayerController*, PlayerController
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestSplit
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestCancelSplit
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestTransferItemToTargetInventory
);

UCLASS(NotBlueprintable, BlueprintType)
class INVENTORYSYSTEM_API UTetrisInventoryEventsHandlerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestDrag OnRequestDrag;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestDrop OnRequestDrop;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestRotate OnRequestRotate;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestEquipUnequip OnRequestEquipUnequip;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestTransferItemToTargetInventory OnRequestTransferItemToTargetInventory;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestDiscard OnRequestDiscard;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestSplit OnRequestSplit;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRequestCancelSplit OnRequestCancelSplit;

private:
	bool bIsSplitting = false;
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

public:
	/**
	 * Called when the player controller changes.
	 * @param NewPlayerController The new player controller.
	 */
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	
	/**
	 * Requests to start dragging an item (e.g., for UI drag-and-drop).
	 */
	UFUNCTION(BlueprintCallable)
	void RequestDrag() const;

	/**
	 * Requests to drop the currently dragged item.
	 */
	UFUNCTION(BlueprintCallable)
	void RequestDrop() const;

	/**
	 * Requests to rotate the currently dragged item.
	 */
	UFUNCTION(BlueprintCallable)
	void RequestRotate() const;

	/**
	 * Requests to equip or unequip the currently selected item.
	 * @param SwapPriority The priority for swapping (default: 0).
	 */
	UFUNCTION(BlueprintCallable)
	void RequestEquipUnequip(const int32 SwapPriority = 0) const;

	/**
	 * Requests to transfer the currently selected item to the target inventory.
	 */
	UFUNCTION(BlueprintCallable)
	void RequestTransferItemToTargetInventory() const;

	/**
	 * Requests to discard the currently selected item.
	 */
	UFUNCTION(BlueprintCallable)
	void RequestDiscard() const;

	/**
	 * Requests to split the currently selected item stack.
	 */
	UFUNCTION(BlueprintCallable)
	void RequestSplit();

	/**
	 * Requests to cancel the current split operation.
	 */
	UFUNCTION(BlueprintCallable)
	void RequestCancelSplit();

	/**
	 * Checks if the system is currently in split mode.
	 * @return True if splitting, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsSplitting() const;
};
