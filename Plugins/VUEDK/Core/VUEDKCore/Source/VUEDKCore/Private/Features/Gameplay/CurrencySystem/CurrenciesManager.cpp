// Copyright VUEDK, Inc. All Rights Reserved.

#include "Features/Gameplay/CurrencySystem/CurrenciesManager.h"
#include "Features/Gameplay/CurrencySystem/Data/SaveData/CurrenciesSaveData.h"

DEFINE_LOG_CATEGORY(LogCurrencySystem);

UCurrenciesManager::UCurrenciesManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TMap<UCurrencyData*, UCurrency*> UCurrenciesManager::GetCurrenciesMap() const
{
	return CurrenciesMap;
}

UCurrency* UCurrenciesManager::GetCurrency(const UCurrencyData* Currency) const
{
	if (!IsValid(Currency))
	{
		UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesManager::GetCurrency: Currency is not valid."));
		return nullptr;
	}

	if (!HasCurrency(Currency))
	{
		UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesManager::GetCurrency: Currency %s is not managed by this manager."), *Currency->GetName());
		return nullptr;
	}

	return CurrenciesMap[Currency];
}

int32 UCurrenciesManager::GetCurrencyAmount(const UCurrencyData* Currency) const
{
	if (!HasCurrency(Currency))
		return 0;

	return CurrenciesMap[Currency]->GetValue();
}

bool UCurrenciesManager::HasCurrency(const UCurrencyData* Currency) const
{
	return CurrenciesMap.Contains(Currency);
}

bool UCurrenciesManager::HasEnoughCurrencyAmount(const UCurrencyData* Currency, const int32 AmountToCheck) const
{
	if (!HasCurrency(Currency))
		return false;

	const int32 MinAmount = Currency->GetMinAmount();
	const int32 Amount = GetCurrencyAmount(Currency);
	return Amount - AmountToCheck >= MinAmount;
}

void UCurrenciesManager::AddCurrency(AActor* Instigator, UCurrencyData* Currency, int32 AmountToAdd, int32& OutRemaining) const
{
	UCurrency* CurrentCurrency = GetCurrency(Currency);
	OutRemaining = 0;
	
	if (!IsValid(CurrentCurrency))
	{
		UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesManager::AddCurrency: Currency %s is not valid."), *Currency->GetName());
		return;
	}

	if (AmountToAdd <= 0)
		return;

	OutRemaining = AmountToAdd;
	AmountToAdd = FMath::Abs(AmountToAdd);
	const int32 MaxAmount = Currency->GetMaxAmount();
	const int32& Amount = CurrentCurrency->GetValue();
	OutRemaining = Amount + AmountToAdd > MaxAmount ? AmountToAdd - (MaxAmount - Amount) : 0;
	CurrentCurrency->ModifyValue(AmountToAdd);
	OnCurrencyAdded.Broadcast(Instigator, Currency, AmountToAdd);
}

void UCurrenciesManager::ConsumeCurrency(AActor* Instigator, UCurrencyData* Currency, const int32 AmountToConsume, int32& OutConsumedAmount) const
{
	UCurrency* CurrentCurrency = GetCurrency(Currency);
	OutConsumedAmount = 0;
	
	if (!IsValid(CurrentCurrency))
	{
		UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesManager::AddCurrency: Currency %s is not valid."), *Currency->GetName());
		return;
	}

	if (AmountToConsume <= 0)
		return;

	OutConsumedAmount = FMath::Abs(AmountToConsume);
	const int32 MinAmount = Currency->GetMinAmount();
	const int32 Amount = CurrentCurrency->GetValue();
	OutConsumedAmount = Amount - AmountToConsume < MinAmount ? Amount - MinAmount : AmountToConsume;
	CurrentCurrency->ModifyValue(-OutConsumedAmount);
	OnCurrencyConsumed.Broadcast(Instigator, Currency, OutConsumedAmount);
}

void UCurrenciesManager::SetCurrencyAmount(UCurrencyData* Currency, const int32 Amount, const bool bNotify) const
{
	UCurrency* CurrentCurrency = GetCurrency(Currency);

	if (!IsValid(CurrentCurrency))
	{
		UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesManager::SetCurrencyAmount: Currency %s is not valid."), *Currency->GetName());
		return;
	}

	CurrentCurrency->SetValue(Amount, bNotify);
}

UCurrency* UCurrenciesManager::FindCurrencyByID(const FGuid& CurrencyID) const
{
	const UCurrencyData* FoundCurrencyData = nullptr;
	for (const UCurrencyData* CurrencyData : Currencies)
	{
		if (CurrencyData->CurrencyID == CurrencyID)
		{
			FoundCurrencyData = CurrencyData;
			break;
		}
	}

	if (IsValid(FoundCurrencyData))
		return GetCurrency(FoundCurrencyData);

	return nullptr;
}

void UCurrenciesManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UCurrenciesManager::Init()
{
	for (UCurrencyData* CurrencyData : Currencies)
	{
		if (!IsValid(CurrencyData))
		{
			UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesManager::Init: Currency %s is not valid."), *CurrencyData->GetName());
			continue;
		}

		UCurrency* NewCurrency = NewObject<UCurrency>();
		NewCurrency->Init(CurrencyData, this);
		CurrenciesMap.Add(CurrencyData, NewCurrency);
	}
}
