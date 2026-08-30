// Copyright VUEDK, Inc. All Rights Reserved.

#include "Features/Gameplay/CurrencySystem/Data/SaveData/CurrenciesSaveData.h"
#include "Features/Gameplay/CurrencySystem/CurrenciesManager.h"

bool UCurrenciesSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const UCurrenciesManager* CurrenciesManager = Cast<UCurrenciesManager>(ObjectToSave);

	if (!IsValid(CurrenciesManager))
		return false;

	for (const TPair<UCurrencyData*, UCurrency*>& Pair : CurrenciesManager->GetCurrenciesMap())
	{
		if (!IsValid(Pair.Key) || !IsValid(Pair.Value))
		{
			UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesSaveData::SaveObjectData: Invalid Currency or CurrencyData."));
			continue;
		}

		const FGuid CurrencyGuid = Pair.Key->CurrencyID;
		SavedCurrencies.Add(CurrencyGuid, Pair.Value->GetValue());
	}
	
	return Super::SaveObjectData(ObjectToSave);
}

bool UCurrenciesSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	const UCurrenciesManager* CurrenciesManager = Cast<UCurrenciesManager>(ObjectToLoad);

	if (!IsValid(CurrenciesManager))
		return false;
	
	for (const TPair<FGuid, int32>& Pair : SavedCurrencies)
	{
		UCurrency* Currency = CurrenciesManager->FindCurrencyByID(Pair.Key);
		if (!IsValid(Currency))
		{
			UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesSaveData::LoadObjectDatatNative: Currency with Id %s not found."), *Pair.Key.ToString());
			continue;
		}
		
		UCurrencyData* Data = Currency->GetCurrencyData();
		if (!IsValid(Data))
		{
			UE_LOG(LogCurrencySystem, Warning, TEXT("UCurrenciesSaveData::LoadObjectDatatNative: CurrencyData for Currency with Id %s is not valid."), *Pair.Key.ToString());
			continue;
		}

		Currency->SetValue(Pair.Value);
	}
	
	return Super::LoadObjectData(ObjectToLoad);
}
