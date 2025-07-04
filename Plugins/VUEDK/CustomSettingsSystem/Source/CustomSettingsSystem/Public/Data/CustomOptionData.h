// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CustomOptionData.generated.h"

USTRUCT(NotBlueprintType)
struct FCustomOptionData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Setting)
	float DefaultValue;
	UPROPERTY(EditAnywhere, Category = Setting)
	float MinValue;
	UPROPERTY(EditAnywhere, Category = Setting)
	float MaxValue;

	FCustomOptionData(): DefaultValue(0.f),
	                     MinValue(0.f),
	                     MaxValue(1.f)
	{
	}
};
