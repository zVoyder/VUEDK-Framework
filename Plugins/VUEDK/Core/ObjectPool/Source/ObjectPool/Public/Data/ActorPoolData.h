// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ActorPoolData.generated.h"

USTRUCT(BlueprintType)
struct FActorPoolData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag PoolTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 InitialSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCapped;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsCapped", EditConditionHides, ClampMin = "1"))
	int32 MaxSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AActor> ActorClass;

	FActorPoolData(): InitialSize(10),
	                  bIsCapped(false),
	                  MaxSize(100)
	{
	}
};
