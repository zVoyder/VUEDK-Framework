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
	UFUNCTION(BlueprintCallable)
	void AddStats(TSet<UStatDataBase*> Stats);
	
	UFUNCTION(BlueprintCallable)
	void AddStat(UStatDataBase* Stat, const float Value);

	UFUNCTION(BlueprintCallable)
	void ClearStats();

	UFUNCTION(BlueprintCallable)
	void RemoveStat(UStatDataBase* Stat);

	UFUNCTION(BlueprintCallable)
	void CopyStats(UStatsContainer* StatsContainer);

	UFUNCTION(BlueprintCallable)
	bool AreStatsEqual(UStatsContainer* StatsContainer) const;
	
	UFUNCTION(BlueprintPure)
	int32 GetValueAsInt(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintPure)
	float GetValueAsFloat(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintPure)
	virtual FString GetValueAsString(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintPure)
	int32 GetStatsLength() const;

	UFUNCTION(BlueprintPure)
	bool HasValue(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "bNotifyEvent"))
	bool TrySetValue(UStatDataBase* Stat, const float Value, const bool bNotifyEvent = true);
	
	UFUNCTION(BlueprintCallable)
	bool TryModifyValue(UStatDataBase* Stat, const float SumValue, const bool bNotifyEvent = true);
	
	UFUNCTION(BlueprintPure)
	TMap<UStatDataBase*, float> GetValues() const;

protected:
	virtual float ValidateStatValue(const float Value, const FFloatRange& Range);
};
