// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatDataBase.generated.h"

UCLASS()
class RPGSYSTEM_API UStatDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FGuid StatId;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MaxLength = 3))
	FName StatCodeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatFullName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText StatDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRepresentAsDecimal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StatDefaultValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FFloatRange StatValueRange;

public:
	UStatDataBase(): StatId(FGuid::NewGuid()),
	                 StatCodeName("AAA"),
	                 bRepresentAsDecimal(false),
	                 StatDefaultValue(0),
	                 StatValueRange(0.0f, 1.0f)
	{
		StatValueRange.SetLowerBound(TRangeBound<float>::Open());
		StatValueRange.SetUpperBound(TRangeBound<float>::Open());
	}
};
