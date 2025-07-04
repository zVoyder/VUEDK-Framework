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
	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|Stats")
	static UStatsBridgeBase* GetPlayerPawnStats(int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category = "VUEDK|RPGSystem|Stats")
	static UStatsBridgeBase* GetPlayerControllerStats(const int32 PlayerIndex);
};
