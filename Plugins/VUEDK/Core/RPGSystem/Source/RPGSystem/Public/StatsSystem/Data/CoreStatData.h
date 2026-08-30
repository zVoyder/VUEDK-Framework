// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatDataBase.h"
#include "Engine/DataAsset.h"
#include "CoreStatData.generated.h"

UCLASS(BlueprintType)
class RPGSYSTEM_API UCoreStatData : public UStatDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MeasurementSymbol;

public:
	UCoreStatData(): MeasurementSymbol("")
	{
	}
};
