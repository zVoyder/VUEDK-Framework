// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Currency.h"
#include "Components/ActorComponent.h"
#include "Data/CurrencyData.h"
#include "Data/SaveData/CurrenciesSaveData.h"
#include "Interfaces/Saveable.h"
#include "CurrenciesManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogCurrencySystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCurrencyUINeedsUpdate
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUEDKCORE_API UCurrenciesManager : public UActorComponent, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrencyUINeedsUpdate OnCurrencyUINeedsUpdate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCurrencyData*> Currencies;

private:
	UPROPERTY()
	TMap<UCurrencyData*, UCurrency*> CurrenciesMap;

public:
	UCurrenciesManager();

	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;

	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;

	UFUNCTION(BlueprintPure)
	UCurrency* GetCurrency(const UCurrencyData* Currency) const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrencyAmount(const UCurrencyData* Currency) const;

	UFUNCTION(BlueprintPure)
	bool HasCurrency(const UCurrencyData* Currency) const;

	UFUNCTION(BlueprintPure)
	bool HasEnoughCurrencyAmount(const UCurrencyData* Currency, const int32 AmountToCheck) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void AddCurrency(UCurrencyData* Currency, int32 AmountToAdd, int32& OutRemaining) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ConsumeCurrency(UCurrencyData* Currency, const int32 AmountToConsume, int32& OutConsumedAmount) const;

	UFUNCTION(BlueprintCallable)
	void SetCurrencyAmount(UCurrencyData* Currency, int32 Amount) const;

protected:
	virtual void BeginPlay() override;

private:
	void Init();

	UCurrency* FindCurrencyByID(const FGuid& CurrencyID) const;
};
