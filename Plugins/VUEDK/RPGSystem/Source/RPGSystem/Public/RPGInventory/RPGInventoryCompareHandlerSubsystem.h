// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSystem/Equipment.h"
#include "Items/RPGGearItem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGInventoryCompareHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnRequestCompare,
	URPGGearItem*, ComparingItem,
	bool, bCompare
);

UCLASS()
class RPGSYSTEM_API URPGInventoryCompareHandlerSubsystem : public UGameInstanceSubsystem
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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Compare")
	void SetTargetEquipmentForCompare(UEquipment* InComparingEquipment);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Compare")
	void SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|Compare")
	void RequestCompare(const bool bCompare);

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|Compare")
	bool IsComparing() const;

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|Compare")
	UEquipment* GetTargetComparingEquipment() const;
	
	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|Compare")
	URPGGearItem* GetComparingItem();

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|Compare")
	UEquipSlotKey* GetTargetComparingEquipSlotKey() const;
};
