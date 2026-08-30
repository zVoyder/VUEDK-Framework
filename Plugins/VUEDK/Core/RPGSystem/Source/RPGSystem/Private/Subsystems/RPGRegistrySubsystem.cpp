// Copyright VUEDK, Inc. All Rights Reserved.

#include "Subsystems/RPGRegistrySubsystem.h"
#include "RPGSystemSettings.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"
#include "RPGItemsGeneration/RPGItemsGenerator.h"

void URPGRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const URPGSystemSettings* RPGSystemSettings = GetDefault<URPGSystemSettings>())
	{
		DefaultRarityLevel = RPGSystemSettings->DefaultRarityLevel.LoadSynchronous();
		
		for (const TSoftObjectPtr<UCoreStatData>& Stat : RPGSystemSettings->Stats)
		{
			UCoreStatData* LoadedStat = Stat.LoadSynchronous();
			if (IsValid(LoadedStat))
				CoreStats.Add(LoadedStat->StatId, LoadedStat);
		}
		
		for (const TSoftObjectPtr<URPGRarityLevelData>& RarityLevel : RPGSystemSettings->RarityLevels)
		{
			URPGRarityLevelData* LoadedRarityLevel = RarityLevel.LoadSynchronous();
			if (IsValid(LoadedRarityLevel))
				RarityLevels.Add(LoadedRarityLevel->RarityId, LoadedRarityLevel);
		}
	}

	URPGInventoriesUtility::Init(this);
	URPGItemsGenerator::Init(this);
}

URPGRarityLevelData* URPGRegistrySubsystem::GetItemRarityById(const FGuid RarityId) const
{
	if (URPGRarityLevelData* const* FoundRarity = RarityLevels.Find(RarityId))
		return *FoundRarity;

	return nullptr;
}

UCoreStatData* URPGRegistrySubsystem::GetItemStatById(const FGuid StatId) const
{
	if (UCoreStatData* const* FoundStat = CoreStats.Find(StatId))
		return *FoundStat;
	
	return nullptr;
}

URPGRarityLevelData* URPGRegistrySubsystem::GetDefaultRarityLevel() const
{
	return DefaultRarityLevel;
}

TMap<FGuid, URPGRarityLevelData*> URPGRegistrySubsystem::GetRarityLevels() const
{
	return RarityLevels;
}

TMap<FGuid, UCoreStatData*> URPGRegistrySubsystem::GetCoreStats() const
{
	return CoreStats;
}
