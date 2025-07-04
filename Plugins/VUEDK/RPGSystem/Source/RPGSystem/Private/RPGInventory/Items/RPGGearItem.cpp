// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Items/RPGGearItem.h"
#include "Factories/ISFactory.h"
#include "Factories/RPGFactory.h"
#include "RPGInventory/Data/RPGGearItemData.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

URPGGearItem::URPGGearItem(): GearStatsContainer(nullptr)
{
}

void URPGGearItem::Init(UObject* WorldContextObject, UItemDataBase* Data)
{
	GearStatsContainer = NewObject<URPGGearItemStatsContainer>(this, TEXT("GearStatsContainer"));
	Super::Init(WorldContextObject, Data);
}

FRPGGearItemSaveData URPGGearItem::CreateRPGGearItemSaveData() const
{
	FRPGGearItemSaveData GearSaveData;
	GearSaveData.RPGItemSaveData = CreateRPGItemSaveData();

	for (const auto& StatModifier : GearStatsContainer->GetValues())
		GearSaveData.GearStats.Add(StatModifier.Key->StatID, StatModifier.Value);

	return GearSaveData;
}

void URPGGearItem::LoadRPGGearItemSaveData(URPGInventory* LoadingInventory, FRPGGearItemSaveData& GearSaveData)
{
	for (const auto& Stats : GearSaveData.GearStats)
	{
		if (UCoreStatData* StatData = URPGInventoriesUtility::GetItemStatByID(Stats.Key); IsValid(StatData))
			GearStatsContainer->AddStat(StatData, Stats.Value);
		else
			UE_LOG(LogTemp, Error, TEXT("Failed to load RPGGearItem bonus stat with ID: %s"), *Stats.Key.ToString());
	}

	LoadRPGItemSaveData(LoadingInventory, GearSaveData.RPGItemSaveData);
}

void URPGGearItem::AddItemStatWithOperation(UCoreStatData* Stat, UStatOperation* Operation)
{
	auto CalculateResult = [this](const UCoreStatData* LocalStat, UStatOperation* LocalOperation) -> float
	{
		const UStatOperation* NewOperation = URPGFactory::CreateItemStatOperation(LocalOperation, this);
		return NewOperation->GetResultOperation();
	};

	if (const float Value = CalculateResult(Stat, Operation); !FMath::IsWithinInclusive(Value, MinValidStatValue, MaxValidStatValue))
		GearStatsContainer->AddStat(Stat, Value);
}

void URPGGearItem::AddItemStatWithValue(UCoreStatData* Stat, const float Value) const
{
	if (!FMath::IsWithinInclusive(Value, MinValidStatValue, MaxValidStatValue))
		GearStatsContainer->AddStat(Stat, Value);
}

URPGGearItemData* URPGGearItem::GetRPGGearItemData() const
{
	return Cast<URPGGearItemData>(ItemData);
}

bool URPGGearItem::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	if (!Super::CanStackItem_Implementation(OtherItem))
		return false;

	const URPGGearItem* GearItem = Cast<URPGGearItem>(OtherItem);

	if (GearItem == nullptr)
		return false;


	return GearStatsContainer->AreStatsEqual(GearItem->GearStatsContainer);
}
