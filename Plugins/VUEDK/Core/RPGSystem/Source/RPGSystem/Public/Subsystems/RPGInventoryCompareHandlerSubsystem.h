// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSystem/Equipment.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGInventoryCompareHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnRequestCompare,
	URPGGearItem*, ComparingItem,
	bool, bCompare
);

UCLASS()
class RPGSYSTEM_API URPGInventoryCompareHandlerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnRequestCompare OnRequestCompare;

private:
	UPROPERTY()
	UEquipment* TargetComparingEquipment;
	UPROPERTY()
	URPGGearItem* TargetComparingItem;
	UPROPERTY()
	UEquipSlotKey* TargetEquipSlotKey;

private:
	bool bIsComparing = false;
	
public:
	/**
	 * Called when the player controller changes.
	 * @param NewPlayerController The new player controller instance.
	 */
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	
	/**
	 * Sets the target equipment to be used for comparison.
	 * @param InComparingEquipment The equipment to compare.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|Compare")
	void SetTargetEquipmentForCompare(UEquipment* InComparingEquipment);
	
	/**
	 * Sets the target equipment slot key to be used for comparison.
	 * @param EquipSlotKey The equipment slot key to compare.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|Compare")
	void SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey);
	
	/**
	 * Requests a comparison operation.
	 * @param bCompare If true, initiates the comparison; if false, cancels it.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|Compare")
	void RequestCompare(const bool bCompare);

	/**
	 * Checks if a comparison is currently in progress.
	 * @return true if comparing, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|Compare")
	bool IsComparing() const;

	/**
	 * Gets the equipment currently being compared.
	 * @return Pointer to the UEquipment being compared, or nullptr if none.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|Compare")
	UEquipment* GetTargetComparingEquipment() const;
	
	/**
	 * Gets the gear item currently being compared.
	 * @return Pointer to the URPGGearItem being compared, or nullptr if none.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|Compare")
	URPGGearItem* GetComparingItem();

	/**
	 * Gets the equipment slot key currently being compared.
	 * @return Pointer to the UEquipSlotKey being compared, or nullptr if none.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|Compare")
	UEquipSlotKey* GetTargetComparingEquipSlotKey() const;
};
