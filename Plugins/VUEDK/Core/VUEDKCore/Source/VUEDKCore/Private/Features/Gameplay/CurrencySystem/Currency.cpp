// Copyright VUEDK, Inc. All Rights Reserved.

#include "Features/Gameplay/CurrencySystem/Currency.h"
#include "Features/Gameplay/CurrencySystem/CurrenciesManager.h"

void UCurrency::Init(UCurrencyData* InCurrencyData, UCurrenciesManager* InCurrenciesManager)
{
	if (!IsValid(InCurrencyData))
	{
		UE_LOG(LogCurrencySystem, Error, TEXT("UCurrency::Init: Invalid CurrencyData provided."));
		return;
	}

	if (!IsValid(InCurrenciesManager))
	{
		UE_LOG(LogCurrencySystem, Error, TEXT("UCurrency::Init: Invalid CurrenciesManager provided."));
		return;
	}

	CurrencyData = InCurrencyData;
	CurrenciesManager = InCurrenciesManager;
	
	MinValue = CurrencyData->GetMinAmount();
	MaxValue = CurrencyData->GetMaxAmount();
	CurrentValue = FMath::Clamp(CurrencyData->StartingAmount, MinValue, MaxValue);
}

bool UCurrency::IsCapped() const
{
	return CurrentValue >= MaxValue;
}

bool UCurrency::IsEmpty() const
{
	return CurrentValue <= MinValue;
}

int32 UCurrency::GetValue() const
{
	return CurrentValue;
}

UCurrencyData* UCurrency::GetCurrencyData() const
{
	return CurrencyData;
}

void UCurrency::SetValue(const int32 NewValue, const bool bNotify)
{
	const int32 OldValue = CurrentValue;
	CurrentValue = FMath::Clamp(NewValue, MinValue, MaxValue);

	if (!bNotify)
		return;
	
	if (OldValue == CurrentValue)
		return;

	OnCurrencyChanged.Broadcast(OldValue, CurrentValue);

	if (CurrentValue > OldValue)
		OnCurrencyIncreased.Broadcast(CurrentValue - OldValue, OldValue, CurrentValue);
	else
		OnCurrencyDecreased.Broadcast(OldValue - CurrentValue, OldValue, CurrentValue);

	if (IsEmpty())
		OnCurrencyEmptied.Broadcast(CurrentValue);

	if (IsCapped())
		OnCurrencyCapped.Broadcast(CurrentValue);
}

void UCurrency::ModifyValue(const int32 Amount, const bool bNotify)
{
	const int32 NewValue = CurrentValue + Amount;
	SetValue(NewValue, bNotify);
}

void UCurrency::Empty()
{
	SetValue(MinValue);
}

void UCurrency::Full()
{
	SetValue(MaxValue);
}

bool UCurrency::Check() const
{
	return IsValid(CurrencyData) && IsValid(CurrenciesManager);
}
