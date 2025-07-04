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

	/**
	 * Creates and returns a save data object representing the current state of currencies.
	 * @return Pointer to the created USaveData instance.
	 */
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;

	/**
	 * Loads the state of currencies from the provided save data.
	 * @param SavedData - The save data to load from.
	 * @return true if the data was loaded successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;

	/**
	 * Gets the currency instance for the specified currency data.
	 * @param Currency - The currency data to retrieve.
	 * @return Pointer to the UCurrency instance, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure)
	UCurrency* GetCurrency(const UCurrencyData* Currency) const;

	/**
	 * Gets the amount of the specified currency.
	 * @param Currency - The currency data to query.
	 * @return The amount of the specified currency.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetCurrencyAmount(const UCurrencyData* Currency) const;

	/**
	 * Checks if the specified currency exists in the manager.
	 * @param Currency - The currency data to check.
	 * @return true if the currency exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasCurrency(const UCurrencyData* Currency) const;

	/**
	 * Checks if the specified currency has at least the given amount.
	 * @param Currency - The currency data to check.
	 * @param AmountToCheck - The minimum amount to check for.
	 * @return true if the currency has at least the specified amount, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasEnoughCurrencyAmount(const UCurrencyData* Currency, const int32 AmountToCheck) const;

	/**
	 * Adds the specified amount to the given currency.
	 * @param Currency - The currency data to add to.
	 * @param AmountToAdd - The amount to add.
	 * @param OutRemaining - Output parameter for the remaining amount after addition.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void AddCurrency(UCurrencyData* Currency, int32 AmountToAdd, int32& OutRemaining) const;

	/**
	 * Consumes the specified amount from the given currency.
	 * @param Currency - The currency data to consume from.
	 * @param AmountToConsume - The amount to consume.
	 * @param OutConsumedAmount - Output parameter for the amount actually consumed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ConsumeCurrency(UCurrencyData* Currency, const int32 AmountToConsume, int32& OutConsumedAmount) const;

	/**
	 * Sets the amount of the specified currency.
	 * @param Currency - The currency data to set.
	 * @param Amount - The amount to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCurrencyAmount(UCurrencyData* Currency, int32 Amount) const;

protected:
	/**
	 * Called when the component begins play. Used for initialization logic.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * Initializes the currencies manager.
	 */
	void Init();

	/**
	 * Finds a currency by its unique identifier.
	 * @param CurrencyID - The unique identifier of the currency.
	 * @return Pointer to the UCurrency instance if found, nullptr otherwise.
	 */
	UCurrency* FindCurrencyByID(const FGuid& CurrencyID) const;
};
