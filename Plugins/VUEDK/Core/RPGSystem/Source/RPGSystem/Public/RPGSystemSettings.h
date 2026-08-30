// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "RPGSystemSettings.generated.h"

UCLASS(Config = RPGSystemSettings, DefaultConfig, meta = (DisplayName = "RPG System Settings"))
class RPGSYSTEM_API URPGSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Stats Registry")
	TSet<TSoftObjectPtr<UCoreStatData>> Stats;
	UPROPERTY(Config, EditAnywhere, Category = "Rarity Levels Registry")
	TSoftObjectPtr<URPGRarityLevelData> DefaultRarityLevel;
	UPROPERTY(Config, EditAnywhere, Category = "Rarity Levels Registry")
	TSet<TSoftObjectPtr<URPGRarityLevelData>> RarityLevels;

private:
	URPGSystemSettings();
};
