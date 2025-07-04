// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/BridgeStatOperation.h"
#include "StatsSystem/StatsBridgeBase.h"

void UBridgeStatOperation::Init(UObject* InPayload, const float InModifierValue, UCoreStatData* InCoreStatData)
{
	CoreStatData = InCoreStatData;
	UStatOperation::Init(InPayload, InModifierValue);
}

UStatsBridgeBase* UBridgeStatOperation::GetStatsBridge() const
{
	return Cast<UStatsBridgeBase>(Payload);
}

float UBridgeStatOperation::GetCoreStatDefaultValue() const
{
	if (!CoreStatData)
		return 0.0f;

	return CoreStatData->StatDefaultValue;
}

float UBridgeStatOperation::GetValueOfSpecialStat(const USpecialStatData* SpecialStat) const
{
	const UStatsBridgeBase* StatsBridge = GetStatsBridge();

	if (!StatsBridge || !SpecialStat)
		return 0.0f;

	return StatsBridge->SpecialStatsContainer->GetValueAsFloat(SpecialStat);
}
