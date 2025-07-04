// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGItemsGeneration/RPGItemsGenerator.h"
#include "Utility/ISInventoriesUtility.h"
#include "Factories/RPGFactory.h"
#include "RPGInventory/RPGInventoriesManager.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

bool URPGItemsGenerator::TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("URPGItemsGenerator::TryGenerateRPGItem: Item is not valid. Cannot generate item."));
		return false;
	}

	if (!IsValid(GenerationData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("URPGItemsGenerator::TryGenerateRPGItem: GenerationData is not valid. Item %s cannot be generated."), *Item->GetItemFullName().ToString());
		return false;
	}

	Item->VisualDetails = GenerateVisualDetails(GenerationData->ItemVisualDetailsSet);
	const URPGInventoriesManager* InventoriesManager = URPGInventoriesUtility::GetRPGInventoriesManager();
	if (!IsValid(InventoriesManager))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("URPGItemsGenerator::TryGenerateRPGItem: RPGInventoriesManager not found. Item %s cannot be generated."), *Item->GetItemFullName().ToString());
		return false;
	}

	if (Item->GetRPGItemData()->bUseRarity)
		Item->RarityLevel = GenerateRarityLevel(InventoriesManager->ItemsRarities);
	
	return true;
}

bool URPGItemsGenerator::TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData)
{
	if (!TryGenerateRPGItem(GearItem, GenerationData))
		return false;

	GenerateItemStatsModifiers(GearItem, GenerationData);
	return true;
}

bool URPGItemsGenerator::TryGenerateRPGGearItemWithFixedStats(URPGGearItem* GearItem, const URPGGearItemFixedStatsGenerationData* GenerationData)
{
	if (!TryGenerateRPGItem(GearItem, GenerationData))
		return false;

	GenerateItemStatsModifiersWithFixedStats(GearItem, GenerationData->Stats);
	return true;
}

FRPGItemVisualDetails URPGItemsGenerator::GenerateVisualDetails(const URPGItemVisualSetData* Set)
{
	if (!IsValid(Set))
		return FRPGItemVisualDetails();

	TArray<FRPGItemVisualDetails> Visuals = Set->Visuals.Array();
	return Visuals[FMath::RandRange(0, Visuals.Num() - 1)];
}

URPGRarityLevelData* URPGItemsGenerator::GenerateRarityLevel(const URPGItemsRaritiesData* Set)
{
	TArray<URPGRarityLevelData*> RarityLevels = Set->RarityLevels.Array();

	const float RandomValue = FMath::FRand();
	float CumulativeProbability = 0.0f;

	for (int32 i = 0; i < RarityLevels.Num(); i++)
	{
		CumulativeProbability += RarityLevels[i]->Probability;
		if (RandomValue <= CumulativeProbability)
			return RarityLevels[i];
	}

	return nullptr;
}

void URPGItemsGenerator::GenerateItemStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData)
{
	TMap<UCoreStatData*, int32> StatsModifiers = TMap<UCoreStatData*, int32>();

	for (const auto& StatModifier : GenerationData->StatsModifiers)
		Item->AddItemStatWithOperation(StatModifier.Key, StatModifier.Value);

	for (const auto& AdditionalStatModifier : GenerationData->AdditionalStatsModifiers)
	{
		if (FMath::FRand() * 100.0f <= AdditionalStatModifier.Value.Probability)
			Item->AddItemStatWithOperation(AdditionalStatModifier.Key, AdditionalStatModifier.Value.StatOperation);
	}
}

void URPGItemsGenerator::GenerateItemStatsModifiersWithFixedStats(const URPGGearItem* Item, const TArray<FFixedStat>& Stats)
{
	for (const auto& Stat : Stats)
		Item->AddItemStatWithValue(Stat.StatData, Stat.Value);
}
