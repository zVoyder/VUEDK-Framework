// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/RPGFactory.h"
#include "RPGSystem.h"
#include "Factories/ISFactory.h"
#include "RPGInventory/Data/RPGGearItemData.h"
#include "RPGInventory/Data/RPGItemData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGInventory/Items/RPGItem.h"
#include "RPGItemsGeneration/RPGItemsGenerator.h"
#include "StatsSystem/StatsBridgeBase.h"

URPGItem* URPGFactory::CreateRPGGenericItem(UObject* WorldContextObject, URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, int32& OutOverflow, const bool bBuildItem, const int32 Quantity)
{
	if (Data == nullptr)
		return nullptr;

	URPGItem* Item = Cast<URPGItem>(UISFactory::CreateItem(WorldContextObject, Data, OutOverflow, Quantity));
	if (!IsValid(Item))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGFactory::CreateRPGGenericItem: Failed to create RPG Item."));
		return nullptr;
	}

	if (bBuildItem)
	{
		if (!URPGItemsGenerator::TryGenerateRPGItem(Item, GenerationData))
			UE_LOG(LogRPGSystem, Warning, TEXT("URPGFactory::CreateRPGGenericItem: GenerationData not provided for RPG Item %s."), *Item->GetItemFullName().ToString());
	}

	Item->NativeOnPostGeneration();
	return Item;
}

URPGGearItem* URPGFactory::CreateRPGGearItem(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem)
{
	URPGGearItem* GearItem = Cast<URPGGearItem>(UISFactory::CreateSingleItem(WorldContextObject, Data));

	if (!IsValid(GearItem))
		return nullptr;

	if (bBuildItem)
		URPGItemsGenerator::TryGenerateRPGGearItem(GearItem, GenerationData);

	GearItem->NativeOnPostGeneration();
	return GearItem;
}

URPGGearItem* URPGFactory::CreateRPGGearItemWithGenerationSet(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemGenerationSet* GenerationSet)
{
	if (!IsValid(GenerationSet))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGFactory::CreateRPGGearItemWithGenerationSet: Invalid GenerationSet."));
		return nullptr;
	}

	if (GenerationSet->Generations.IsEmpty())
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGFactory::CreateRPGGearItemWithGenerationSet: GenerationSet has no generations."));
		return nullptr;
	}

	URPGItemBaseGenerationData* GenerationData = GenerationSet->Generations[FMath::RandRange(0, GenerationSet->Generations.Num() - 1)];

	if (!IsValid(GenerationData))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGFactory::CreateRPGGearItemWithGenerationSet: Invalid GenerationData in GenerationSet."));
		return nullptr;
	}
	
	if (GenerationData->IsA(URPGGearItemGenerationData::StaticClass()))
		return CreateRPGGearItem(WorldContextObject, Data, Cast<URPGGearItemGenerationData>(GenerationData));

	if (GenerationData->IsA(URPGGearItemFixedStatsGenerationData::StaticClass()))
		return CreateRPGGearItemWithFixedStats(WorldContextObject, Data, Cast<URPGGearItemFixedStatsGenerationData>(GenerationData));

	return nullptr;
}

URPGGearItem* URPGFactory::CreateRPGGearItemWithFixedStats(UObject* WorldContextObject, URPGGearItemData* Data, const URPGGearItemFixedStatsGenerationData* GenerationData)
{
	URPGGearItem* GearItem = Cast<URPGGearItem>(UISFactory::CreateSingleItem(WorldContextObject, Data));

	if (!IsValid(GearItem))
		return nullptr;

	if (!URPGItemsGenerator::TryGenerateRPGGearItemWithFixedStats(GearItem, GenerationData))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGFactory::CreateRPGGearItemWithFixedStats: Failed to generate RPG Gear Item Base."));
		return nullptr;
	}

	GearItem->NativeOnPostGeneration();
	return GearItem;
}

UStatOperation* URPGFactory::CreateItemStatOperation(UStatOperation* Operation, URPGItem* Item)
{
	if (!IsValid(Operation))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("URPGFactory::CreateItemStatOperation: Invalid Operation."));
		return nullptr;
	}

	UStatOperation* NewOperation = DuplicateObject<UStatOperation>(Operation, Item);
	if (!IsValid(NewOperation))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("URPGFactory::CreateItemStatOperation: Failed to create new operation for item %s."), *Item->GetItemFullName().ToString());
		return nullptr;
	}
	
	const URPGRarityLevelData* RarityLevel = Item->RarityLevel;
	if (!IsValid(RarityLevel))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("URPGFactory::CreateItemStatOperation: Invalid RarityLevel for item %s."), *Item->GetItemFullName().ToString());
		return nullptr;
	}
	
	NewOperation->Init(Item, RarityLevel->RarityValue);
	return NewOperation;
}

UBridgeStatOperation* URPGFactory::CreateBridgeStatOperation(UStatsBridgeBase* Bridge, UCoreStatData* InCoreStatData, USpecialStatData* SpecialStat, const TSubclassOf<UBridgeStatOperation> OperationClass)
{
	if (!IsValid(Bridge))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("URPGFactory::CreateBridgeStatOperation: Invalid Bridge"));
		return nullptr;
	}

	if (!IsValid(SpecialStat))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("URPGFactory::CreateBridgeStatOperation: Invalid SpecialStat"));
		return nullptr;
	}

	UBridgeStatOperation* NewOperation = NewObject<UBridgeStatOperation>(Bridge, OperationClass);
	if (!IsValid(NewOperation))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("URPGFactory::CreateBridgeStatOperation: Failed to create new operation for bridge %s."), *Bridge->GetName());
		return nullptr;
	}
	
	const float Value = Bridge->SpecialStatsContainer->GetValueAsFloat(SpecialStat);
	NewOperation->Init(Bridge, Value, InCoreStatData);
	return NewOperation;
}
