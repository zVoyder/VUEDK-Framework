// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/Containers/CoreStatsContainer.h"

void UCoreStatsContainer::AddCoreStats(TSet<UCoreStatData*> CoreStats)
{
	for (UCoreStatData* CoreStat : CoreStats)
		AddStat(CoreStat, CoreStat->StatDefaultValue);
}

TMap<UCoreStatData*, float> UCoreStatsContainer::GetCoreStatsValues()
{
	TMap<UCoreStatData*, float> CoreStatsValues;
	
	for (auto& Stat : Values)
	{
		if (UCoreStatData* CoreStat = Cast<UCoreStatData>(Stat.Key))
			CoreStatsValues.Add(CoreStat, Stat.Value);
	}
	
	return CoreStatsValues;
}

FString UCoreStatsContainer::GetValueAsString(const UStatDataBase* Stat) const
{
	FString Value = Super::GetValueAsString(Stat);
	
	if (Value.IsEmpty())
		return Value;
	
	const UCoreStatData* CoreStat = Cast<UCoreStatData>(Stat);
	if (!IsValid(CoreStat))
		return Value;
	
	return Value + " " + CoreStat->MeasurementSymbol;
}
