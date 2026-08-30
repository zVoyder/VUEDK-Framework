// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsSystem/StatsBridgeBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatsUtility.generated.h"

UCLASS()
class RPGSYSTEM_API UStatsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Gets the stats bridge for the player pawn at the specified player index.
	 * @param PlayerIndex The index of the player.
	 * @return Pointer to the UStatsBridgeBase instance for the player pawn, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|Stats")
	static UStatsBridgeBase* GetPlayerPawnStats(int32 PlayerIndex);

	/**
	 * Gets the stats bridge for the player controller at the specified player index.
	 * @param PlayerIndex The index of the player controller.
	 * @return Pointer to the UStatsBridgeBase instance for the player controller, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "RPGSystem|Stats")
	static UStatsBridgeBase* GetPlayerControllerStats(const int32 PlayerIndex);
};
