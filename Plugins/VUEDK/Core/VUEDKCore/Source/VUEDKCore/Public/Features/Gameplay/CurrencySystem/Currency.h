// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CurrencyData.h"
#include "UObject/Object.h"
#include "Currency.generated.h"

class UCurrenciesManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCurrencyChanged,
	int32,
	OldValue,
	int32,
	NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnCurrencyIncreased,
	int32,
	IncreasedAmount,
	int32,
	OldValue,
	int32,
	NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnCurrencyDecreased,
	int32,
	DecreasedAmount,
	int32,
	OldValue,
	int32,
	NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCurrencyCapped,
	int32,
	CurrentValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCurrencyEmptied,
	int32,
	CurrentValue
);

UCLASS(BlueprintType)
class VUEDKCORE_API UCurrency : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrencyChanged OnCurrencyChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrencyIncreased OnCurrencyIncreased;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrencyDecreased OnCurrencyDecreased;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrencyCapped OnCurrencyCapped;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrencyEmptied OnCurrencyEmptied;

private:
	UPROPERTY()
	UCurrenciesManager* CurrenciesManager = nullptr;
	UPROPERTY()
	UCurrencyData* CurrencyData = nullptr;
	int32 CurrentValue = 0;
	int32 MinValue = 0;
	int32 MaxValue = 1;

public:
	/**
	 * Initializes the currency with the specified currency data and currencies manager.
	 * @param InCurrencyData The currency data asset to associate with this currency.
	 * @param InCurrenciesManager The currencies manager that owns this currency.
	 */
	void Init(UCurrencyData* InCurrencyData, UCurrenciesManager* InCurrenciesManager);

	/**
	 * Checks if the currency value is empty (zero or at minimum).
	 * @return true if the currency is empty, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	/**
	 * Checks if the currency value is capped (at maximum).
	 * @return true if the currency is capped, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsCapped() const;

	/**
	 * Gets the current value of the currency.
	 * @return The current value as an integer.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetValue() const;

	UFUNCTION(BlueprintPure)
	UCurrencyData* GetCurrencyData() const;

	/**
	 * Sets the value of the currency.
	 * @param NewValue The new value to set.
	 * @param bNotify Whether to notify listeners of the change.
	 */
	UFUNCTION(BlueprintCallable)
	void SetValue(const int32 NewValue, const bool bNotify = true);

	/**
	 * Modifies the value of the currency by the specified amount.
	 * @param Amount The amount to add or subtract from the current value.
	 * @param bNotify Whether to notify listeners of the change.
	 */
	UFUNCTION(BlueprintCallable)
	void ModifyValue(const int32 Amount, const bool bNotify = true);

	/**
	 * Empties the currency, setting its value to the minimum.
	 */
	UFUNCTION(BlueprintCallable)
	void Empty();

	/**
	 * Fills the currency, setting its value to the maximum.
	 */
	UFUNCTION(BlueprintCallable)
	void Full();

private:
	/**
	 * Checks if the currency is valid and initialized.
	 * @return True if the currency is valid, false otherwise.
	 */
	bool Check() const;
};
