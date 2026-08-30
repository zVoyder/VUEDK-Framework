// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGItemsGeneration/RPGItemsGenerator.h"
#include "RPGSystem.h"
#include "Factories/RPGFactory.h"
#include "RPGInventory/Data/RPGItemData.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

TWeakObjectPtr<URPGRegistrySubsystem> URPGItemsGenerator::RegistrySubsystem = nullptr;

void URPGItemsGenerator::Init(URPGRegistrySubsystem* InRegistrySubsystem)
{
	RegistrySubsystem = InRegistrySubsystem;
}

bool URPGItemsGenerator::TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGItemsGenerator::TryGenerateRPGItem: Item is not valid. Cannot generate item."));
		return false;
	}

	if (!IsValid(GenerationData))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGItemsGenerator::TryGenerateRPGItem: GenerationData is not valid. Item %s cannot be generated."), *Item->GetItemFullName().ToString());
		return false;
	}

	Item->VisualDetails = GenerateVisualDetails(GenerationData->ItemVisualDetailsSet);
	const URPGItemData* RPGItemData = Item->GetRPGItemData();
	if (RPGItemData->bUseRarity && !RPGItemData->bUsePredefinedRarity)
		Item->RarityLevel = GenerateRarityLevel();
	
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

URPGRarityLevelData* URPGItemsGenerator::GenerateRarityLevel()
{
	if (!Check())
		return nullptr;
	
	const float RandomValue = FMath::FRand();
	float CumulativeProbability = 0.0f;
	
	for (const auto& RarityLevel : RegistrySubsystem.Get()->GetRarityLevels())
	{
		CumulativeProbability += RarityLevel.Value->Probability;
		if (RandomValue <= CumulativeProbability)
			return RarityLevel.Value;
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

bool URPGItemsGenerator::Check()
{
	return RegistrySubsystem.IsValid();
}
