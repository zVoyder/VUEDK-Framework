// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGRegistrySubsystem.generated.h"

UCLASS()
class RPGSYSTEM_API URPGRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<URPGRarityLevelData> DefaultRarityLevel;
	UPROPERTY()
	TMap<FGuid, URPGRarityLevelData*> RarityLevels;
	UPROPERTY()
	TMap<FGuid, UCoreStatData*> CoreStats;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	URPGRarityLevelData* GetItemRarityById(const FGuid RarityId) const;
	
	UCoreStatData* GetItemStatById(const FGuid StatId) const;

	UFUNCTION(BlueprintCallable, Category = "RPG System|Registry")
	URPGRarityLevelData* GetDefaultRarityLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPG System|Registry")
	TMap<FGuid, URPGRarityLevelData*> GetRarityLevels() const;

	TMap<FGuid, UCoreStatData*> GetCoreStats() const;
};
