// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsContainer.h"
#include "StatsSystem/Data/SpecialStatData.h"
#include "UObject/Object.h"
#include "SpecialStatsContainer.generated.h"

UCLASS()
class RPGSYSTEM_API USpecialStatsContainer : public UStatsContainer
{
	GENERATED_BODY()

public:
	/**
	 * Adds a set of special stats to the container.
	 * @param SpecialStats - The set of special stat data objects to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddSpecialStats(TSet<USpecialStatData*> SpecialStats);

	/**
	 * Retrieves the current values of all special stats in the container.
	 * @return A map of special stat data objects to their float values.
	 */
	UFUNCTION(BlueprintPure)
	TMap<USpecialStatData*, float> GetSpecialStatsValues();
};
