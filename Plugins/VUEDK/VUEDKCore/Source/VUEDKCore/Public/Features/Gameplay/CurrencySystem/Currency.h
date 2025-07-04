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

	friend class UCurrenciesManager;
	
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
	void Init(UCurrencyData* InCurrencyData, UCurrenciesManager* InCurrenciesManager);

	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	UFUNCTION(BlueprintPure)
	bool IsCapped() const;

	UFUNCTION(BlueprintPure)
	int32 GetValue() const;

	UFUNCTION(BlueprintCallable)
	void SetValue(const int32 NewValue);

	UFUNCTION(BlueprintCallable)
	void ModifyValue(const int32 Amount);

	UFUNCTION(BlueprintCallable)
	void Empty();

	UFUNCTION(BlueprintCallable)
	void Full();

private:
	bool Check() const;
};
