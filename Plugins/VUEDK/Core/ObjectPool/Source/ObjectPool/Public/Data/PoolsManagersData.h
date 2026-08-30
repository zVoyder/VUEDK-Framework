// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.h"
#include "PoolsManagersData.generated.h"

USTRUCT(BlueprintType)
struct FPoolsManagersData
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray<TSoftClassPtr<AGameModeBase>> AllowedGamemodes;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray<FActorPoolData> PoolsData;
};
