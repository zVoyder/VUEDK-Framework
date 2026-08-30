// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsSystem/Data/CoreStatData.h"
#include "UObject/Object.h"
#include "StatsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnStatsValuesChanged
);

UCLASS(BlueprintType)
class RPGSYSTEM_API UStatsContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnStatsValuesChanged OnStatsValuesChanged;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<UStatDataBase*, float> Values;

public:
	/**
	 * Adds a set of stats to the container.
	 * @param Stats The set of stat data objects to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddStats(TSet<UStatDataBase*> Stats);
	
	/**
	 * Adds a single stat to the container with a specified value.
	 * @param Stat The stat data object to add.
	 * @param Value The value to assign to the stat.
	 */
	UFUNCTION(BlueprintCallable)
	void AddStat(UStatDataBase* Stat, const float Value);

	/**
	 * Clears all stats from the container.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearStats();

	/**
	 * Removes a specific stat from the container.
	 * @param Stat The stat data object to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveStat(UStatDataBase* Stat);

	/**
	 * Copies all stats from another stats container into this one.
	 * @param StatsContainer The container to copy stats from.
	 */
	UFUNCTION(BlueprintCallable)
	void CopyStats(UStatsContainer* StatsContainer);

	/**
	 * Checks if the stats in this container are equal to those in another container.
	 * @param StatsContainer The container to compare with.
	 * @return true if the stats are equal, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool AreStatsEqual(UStatsContainer* StatsContainer) const;
	
	/**
	 * Gets the value of a specific stat as an integer.
	 * @param Stat The stat data object to query.
	 * @return The value of the stat as an integer.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetValueAsInt(const UStatDataBase* Stat) const;

	/**
	 * Gets the value of a specific stat as a float.
	 * @param Stat The stat data object to query.
	 * @return The value of the stat as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetValueAsFloat(const UStatDataBase* Stat) const;

	/**
	 * Gets the value of a specific stat as a string.
	 * @param Stat The stat data object to query.
	 * @return The value of the stat as a string.
	 */
	UFUNCTION(BlueprintPure)
	virtual FString GetValueAsString(const UStatDataBase* Stat) const;

	/**
	 * Gets the number of stats in the container.
	 * @return The number of stats in the container.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetStatsLength() const;

	/**
	 * Checks if a specific stat has a value assigned.
	 * @param Stat The stat data object to check.
	 * @return true if the stat has a value, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasValue(const UStatDataBase* Stat) const;

	/**
	 * Tries to set the value of a specific stat.
	 * @param Stat The stat data object to modify.
	 * @param Value The new value to assign to the stat.
	 * @param bNotifyEvent Whether to notify about the value change.
	 * @return true if the value was set successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bNotifyEvent"))
	bool TrySetValue(UStatDataBase* Stat, const float Value, const bool bNotifyEvent = true);
	
	/**
	 * Tries to modify the value of a specific stat by adding a sum to the current value.
	 * @param Stat The stat data object to modify.
	 * @param SumValue The value to add to the current stat value.
	 * @param bNotifyEvent Whether to notify about the value change.
	 * @return true if the value was modified successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryModifyValue(UStatDataBase* Stat, const float SumValue, const bool bNotifyEvent = true);
	
	/**
	 * Gets a map of all stat data objects and their corresponding values.
	 * @return A map of all stat data objects and their values.
	 */
	UFUNCTION(BlueprintPure)
	TMap<UStatDataBase*, float> GetValues() const;

protected:
	/**
	 * Validates a stat value against its defined range.
	 * @param Value The value to validate.
	 * @param Range The range within which the value must fall.
	 * @return The validated value, clamped to the range if necessary.
	 */
	virtual float ValidateStatValue(const float Value, const FFloatRange& Range);
};
