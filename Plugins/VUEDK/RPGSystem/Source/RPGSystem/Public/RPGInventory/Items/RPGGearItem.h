// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "RPGInventory/Stats/RPGGearItemStatsContainer.h"
#include "RPGGearItem.generated.h"

class UStatOperation;
class URPGGearItemData;

constexpr int32 MinValidStatValue = -0.05f;
constexpr int32 MaxValidStatValue = 0.05f;

UCLASS()
class RPGSYSTEM_API URPGGearItem : public URPGItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	URPGGearItemStatsContainer* GearStatsContainer;

	//TODO: Add SpecialStatsContainer for gear items if needed
	
public:
	URPGGearItem();

	virtual void Init(UObject* WorldContextObject, UItemDataBase* Data) override;
	
	FRPGGearItemSaveData CreateRPGGearItemSaveData() const;
	
	void LoadRPGGearItemSaveData(URPGInventory* LoadingInventory, FRPGGearItemSaveData& GearSaveData);
	
	UFUNCTION(BlueprintCallable)
	void AddItemStatWithOperation(UCoreStatData* Stat, UStatOperation* Operation);

	UFUNCTION(BlueprintCallable)
	void AddItemStatWithValue(UCoreStatData* Stat, const float Value) const;
	
	UFUNCTION(BlueprintPure)
	URPGGearItemData* GetRPGGearItemData() const;

	virtual bool CanStackItem_Implementation(UItemBase* OtherItem) const override;
};
