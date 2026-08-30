// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SmartRespawnSnapshot.h"
#include "SmartRespawnRegister.generated.h"

class USmartRespawn;

USTRUCT(BlueprintType)
struct FRespawnLevelValue
{
	GENERATED_BODY()

	// Map representing the respawns registered by the player in a specific level, with the respawn tag as key and the respawn snapshot as value.
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGameplayTag, FSmartRespawnSnapshot> Respawns;
};

USTRUCT(BlueprintType)
struct FRespawnRegisterValue
{
	GENERATED_BODY()

	// Map representing the levels in which the player has registered respawns.
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FString, FRespawnLevelValue> LevelRespawns;
};

USTRUCT(BlueprintType)
struct FSmartRespawnRegister
{
	GENERATED_BODY()

	// Map representing the players that have registered respawns.
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FName, FRespawnRegisterValue> PlayerRespawns;
};
