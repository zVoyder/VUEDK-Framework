// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/SaveData/StatsSaveData.h"
#include "StatsSystem/StatsBridgeBase.h"

bool UStatsSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const UStatsBridgeBase* StatsBridge = Cast<UStatsBridgeBase>(ObjectToSave);
	
	if (!IsValid(StatsBridge))
		return false;

	for (auto& Pair : StatsBridge->SpecialStatsContainer->GetValues())
		SavedSpecialStats.Add(Pair.Key->StatId, Pair.Value);

	for (auto& Pair : StatsBridge->CoreStatsContainer->GetValues())
		SavedCoreStats.Add(Pair.Key->StatId, Pair.Value);

	return Super::SaveObjectData(ObjectToSave);
}

bool UStatsSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	const UStatsBridgeBase* StatsBridge = Cast<UStatsBridgeBase>(ObjectToLoad);
	if (!IsValid(StatsBridge))
		return false;

	for (auto& SpecialStatsMap = SavedSpecialStats; const auto& Pair : SpecialStatsMap)
	{
		if (USpecialStatData* SpecialStatData = StatsBridge->GetSpecialStatById(Pair.Key))
			StatsBridge->SpecialStatsContainer->AddStat(SpecialStatData, Pair.Value);
	}

	for (auto& CoreStatsMap = SavedCoreStats; const auto& Pair : CoreStatsMap)
	{
		if (UCoreStatData* CoreStatData = StatsBridge->GetCoreStatById(Pair.Key))
			StatsBridge->CoreStatsContainer->AddStat(CoreStatData, Pair.Value);
	}
	
	return Super::LoadObjectData(ObjectToLoad);
}
