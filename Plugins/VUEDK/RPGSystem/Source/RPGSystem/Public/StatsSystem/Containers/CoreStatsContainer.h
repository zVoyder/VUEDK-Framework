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
	/**
	 * Adds a set of core stats to the container.
	 * @param CoreStats - The set of core stat data objects to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddCoreStats(TSet<UCoreStatData*> CoreStats);

	/**
	 * Retrieves the current values of all core stats in the container.
	 * @return A map of core stat data objects to their float values.
	 */
	UFUNCTION(BlueprintPure)
	TMap<UCoreStatData*, float> GetCoreStatsValues();
	
	/**
	 * Gets the value of a specific stat as a string.
	 * @param Stat - The stat data object to query.
	 * @return The value of the stat as a string.
	 */
	virtual FString GetValueAsString(const UStatDataBase* Stat) const override;
};
