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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRequestDiscard
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

public:
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	
	UFUNCTION(BlueprintCallable)
	void RequestDrag() const;

	UFUNCTION(BlueprintCallable)
	void RequestDrop() const;

	UFUNCTION(BlueprintCallable)
	void RequestRotate() const;

	UFUNCTION(BlueprintCallable)
	void RequestEquipUnequip(const int32 SwapPriority = 0) const;

	UFUNCTION(BlueprintCallable)
	void RequestTransferItemToTargetInventory() const;

	UFUNCTION(BlueprintCallable)
	void RequestDiscard() const;

	UFUNCTION(BlueprintCallable)
	void RequestSplit();

	UFUNCTION(BlueprintCallable)
	void RequestCancelSplit();

	UFUNCTION(BlueprintPure)
	bool IsSplitting() const;
};
