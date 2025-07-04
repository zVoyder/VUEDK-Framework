// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/Containers/StatsContainer.h"
#include "StatsSystem/StatsBridgeBase.h"

class UStatsSaveData;

void UStatsContainer::AddStats(TSet<UStatDataBase*> Stats)
{
	for (UStatDataBase* Stat : Stats)
		AddStat(Stat, Stat->StatDefaultValue);
}

void UStatsContainer::AddStat(UStatDataBase* Stat, const float Value)
{
	if (!Values.Contains(Stat))
		Values.Add(Stat, Stat->StatDefaultValue);

	TrySetValue(Stat, Value);
}

void UStatsContainer::ClearStats()
{
	Values.Empty();
	OnStatsValuesChanged.Broadcast();
}

void UStatsContainer::RemoveStat(UStatDataBase* Stat)
{
	if (!Values.Contains(Stat))
		return;

	Values.Remove(Stat);
	OnStatsValuesChanged.Broadcast();
}

void UStatsContainer::CopyStats(UStatsContainer* StatsContainer)
{
	if (StatsContainer == nullptr)
		return;
	
	for (const auto& Pair : StatsContainer->Values)
		AddStat(Pair.Key, Pair.Value);

	OnStatsValuesChanged.Broadcast();
}

bool UStatsContainer::AreStatsEqual(UStatsContainer* StatsContainer) const
{
	if (Values.Num() != StatsContainer->Values.Num())
		return false;

	for (const auto& Pair : Values)
	{
		if (!StatsContainer->Values.Contains(Pair.Key) || StatsContainer->Values[Pair.Key] != Pair.Value)
			return false;
	}

	return true;
}

int32 UStatsContainer::GetValueAsInt(const UStatDataBase* Stat) const
{
	if (!IsValid(Stat))
		return 0;

	if (!Values.Contains(Stat))
		return 0;

	return Values[Stat];
}

float UStatsContainer::GetValueAsFloat(const UStatDataBase* Stat) const
{
	if (!IsValid(Stat))
		return 0.f;

	if (!Values.Contains(Stat))
		return 0.f;

	return Values[Stat];
}

FString UStatsContainer::GetValueAsString(const UStatDataBase* Stat) const
{
	if (!IsValid(Stat))
		return "";

	if (!Values.Contains(Stat))
		return "";

	return Stat->bRapresentAsDecimal ? FString::Printf(TEXT("%.2f"), GetValueAsFloat(Stat)) : FString::Printf(TEXT("%d"), GetValueAsInt(Stat));
}

int32 UStatsContainer::GetStatsLength() const
{
	return Values.Num();
}

bool UStatsContainer::HasValue(const UStatDataBase* Stat) const
{
	return IsValid(Stat) && Values.Contains(Stat);
}

bool UStatsContainer::TrySetValue(UStatDataBase* Stat, const float Value, const bool bNotifyEvent)
{
	if (!HasValue(Stat))
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("UStatsContainer::TrySetValue: Stat %s is not found in the container."), *Stat->GetName());
		return false;
	}
		
	const float OldValue = Values[Stat];
	Values[Stat] = ValidateStatValue(Value, Stat->StatValueRange);
	if (OldValue == Values[Stat])
		return false;
	
	if (bNotifyEvent)
		OnStatsValuesChanged.Broadcast();

	return true;
}

bool UStatsContainer::TryModifyValue(UStatDataBase* Stat, const float SumValue, const bool bNotifyEvent)
{
	return TrySetValue(Stat, GetValueAsFloat(Stat) + SumValue, bNotifyEvent);
}

TMap<UStatDataBase*, float> UStatsContainer::GetValues() const
{
	return Values;
}

float UStatsContainer::ValidateStatValue(const float Value, const FFloatRange& Range)
{
	if (Range.GetUpperBound().IsOpen() && Range.GetLowerBound().IsOpen())
		return Value;

	float MinClamp = MIN_flt;
	float MaxClamp = MAX_flt;

	if (Range.GetLowerBound().IsClosed())
		MinClamp = Range.GetLowerBound().IsInclusive() ? Range.GetLowerBound().GetValue() : Range.GetLowerBound().GetValue() + 1;

	if (Range.GetUpperBound().IsClosed())
		MaxClamp = Range.GetUpperBound().IsInclusive() ? Range.GetUpperBound().GetValue() : Range.GetUpperBound().GetValue() - 1;

	return FMath::Clamp(Value, MinClamp, MaxClamp);
}
