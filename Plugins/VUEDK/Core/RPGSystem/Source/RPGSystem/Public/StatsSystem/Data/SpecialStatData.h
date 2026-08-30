// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsSystem/Data/StatDataBase.h"
#include "SpecialStatData.generated.h"

UCLASS(BlueprintType)
class RPGSYSTEM_API USpecialStatData : public UStatDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor StatColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* StatIcon;

public:
	USpecialStatData(): StatColor(),
	                    StatIcon(nullptr)
	{
	}
};
