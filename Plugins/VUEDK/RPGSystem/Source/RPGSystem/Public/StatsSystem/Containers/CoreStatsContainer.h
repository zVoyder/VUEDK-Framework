// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsContainer.h"
#include "CoreStatsContainer.generated.h"

UCLASS()
class RPGSYSTEM_API UCoreStatsContainer : public UStatsContainer
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddCoreStats(TSet<UCoreStatData*> CoreStats);

	UFUNCTION(BlueprintPure)
	TMap<UCoreStatData*, float> GetCoreStatsValues();
	
	virtual FString GetValueAsString(const UStatDataBase* Stat) const override;
};
