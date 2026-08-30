// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoneDamageProcessorData.generated.h"

USTRUCT(BlueprintType)
struct FBoneDamageProcessorData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitBox")
	float DamageMultiplier = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitBox")
	TArray<TSoftClassPtr<class UDamageProcessor>> DamageProcessors;
};
