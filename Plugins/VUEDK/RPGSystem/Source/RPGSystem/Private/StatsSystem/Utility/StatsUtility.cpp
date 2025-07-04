// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/Utility/StatsUtility.h"
#include "StatsSystem/StatsBridgeBase.h"
#include "Kismet/GameplayStatics.h"

UStatsBridgeBase* UStatsUtility::GetPlayerPawnStats(int32 PlayerIndex)
{
	const UWorld* World = GEngine->GetWorld();
	
	if (!World)
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("GetPlayerStats(), World is nullptr"));
		return nullptr;
	}

	const APawn* Pawn = UGameplayStatics::GetPlayerPawn(World, 0);

	if (!Pawn)
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("GetPlayerStats(), Character is nullptr"));
		return nullptr;
	}

	UStatsBridgeBase* CharacterStats = Pawn->FindComponentByClass<UStatsBridgeBase>();
	return CharacterStats;
}

UStatsBridgeBase* UStatsUtility::GetPlayerControllerStats(const int32 PlayerIndex)
{
	const UWorld* World = GEngine->GetWorld();
	
	if (!World)
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("GetPlayerStats(), World is nullptr"));
		return nullptr;
	}

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);

	if (!PlayerController)
	{
		UE_LOG(LogStatsSystem, Warning, TEXT("GetPlayerStats(), PlayerController is nullptr"));
		return nullptr;
	}

	UStatsBridgeBase* PlayerControllerStats = PlayerController->FindComponentByClass<UStatsBridgeBase>();
	return PlayerControllerStats;
}
