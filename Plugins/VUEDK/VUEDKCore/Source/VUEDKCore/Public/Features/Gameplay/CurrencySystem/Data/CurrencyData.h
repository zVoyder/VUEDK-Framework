// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CurrencyData.generated.h"

UCLASS()
class VUEDKCORE_API UCurrencyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FGuid CurrencyID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CurrencyName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CurrencyDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush CurrencyIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StartingAmount = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FInt32Range CurrencyRange;

	UCurrencyData(): CurrencyID(FGuid::NewGuid())
	{
		CurrencyRange.SetLowerBound(FInt32Range::BoundsType::Inclusive(0));
		CurrencyRange.SetUpperBound(FInt32Range::BoundsType::Inclusive(100));
	}
	
	int32 GetMaxAmount() const
	{
		return CurrencyRange.GetUpperBound().IsClosed() ? CurrencyRange.GetUpperBoundValue() : MAX_int32;
	}

	UFUNCTION(BlueprintPure)
	int32 GetMinAmount() const
	{
		return CurrencyRange.GetLowerBound().IsClosed() ? CurrencyRange.GetLowerBoundValue() : MIN_int32;
	}
};
