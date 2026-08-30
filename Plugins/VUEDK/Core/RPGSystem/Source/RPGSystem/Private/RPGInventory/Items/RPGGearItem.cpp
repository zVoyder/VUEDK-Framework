// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Items/RPGGearItem.h"
#include "Factories/ISFactory.h"
#include "Factories/RPGFactory.h"
#include "RPGInventory/Data/RPGGearItemData.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

URPGGearItem::URPGGearItem(): GearStatsContainer(nullptr)
{
}

void URPGGearItem::Init(const TObjectPtr<UWorld> InWorld, UItemDataBase* Data)
{
	GearStatsContainer = NewObject<URPGGearItemStatsContainer>(this, TEXT("GearStatsContainer"));
	Super::Init(InWorld, Data);
}

void URPGGearItem::AddItemStatWithOperation(UCoreStatData* Stat, UStatOperation* Operation)
{
	auto CalculateResult = [this](const UCoreStatData* LocalStat, UStatOperation* LocalOperation) -> float
	{
		const UStatOperation* NewOperation = URPGFactory::CreateItemStatOperation(LocalOperation, this);
		if (!IsValid(NewOperation))
			return 0.f;
		
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
