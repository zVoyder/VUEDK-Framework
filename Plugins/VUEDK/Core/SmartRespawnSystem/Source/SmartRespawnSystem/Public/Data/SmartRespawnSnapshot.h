// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SmartRespawnSnapshot.generated.h"

class USmartRespawnBehaviour;

class USmartRespawn;

USTRUCT(BlueprintType)
struct FSmartRespawnSnapshot
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 Priority;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FTransform RespawnTransform;
	UPROPERTY(SaveGame, BlueprintReadWrite, AdvancedDisplay)
	TSoftObjectPtr<AActor> Owner;
	UPROPERTY(SaveGame, BlueprintReadWrite, AdvancedDisplay)
	TSoftObjectPtr<USmartRespawn> SmartRespawn;
	UPROPERTY(SaveGame, BlueprintReadWrite, AdvancedDisplay)
	TSoftClassPtr<USmartRespawnBehaviour> CustomRespawnBehaviour;
	UPROPERTY(SaveGame)
	FDateTime Timestamp;
	
	FSmartRespawnSnapshot() : Priority(0),
	                          RespawnTransform(FTransform::Identity),
	                          Timestamp(FDateTime::MinValue())
	{
	}
};
