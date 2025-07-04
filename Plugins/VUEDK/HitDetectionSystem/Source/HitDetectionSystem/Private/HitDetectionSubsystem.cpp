// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitDetectionSubsystem.h"
#include "HitDetectionSystem.h"

void UHitDetectionSubsystem::CallForHit(AController* InstigatedBy, const FVector& HitLocation, const FVector& HitNormal, const FName& BoneName, float DamageMultiplier, float TotalDamage, float BaseDamage) const
{
	const UWorld* World = GetWorld();

	if (!World || !InstigatedBy)
	{
		UE_LOG(LogHitDetectionSystem, Warning, TEXT("HitDetectionSubsystem::CallForHit - Invalid parameters."));
		return;
	}

	if (InstigatedBy == GetLocalPlayer()->GetPlayerController(World))
	{
		OnPlayerPerformedHit.Broadcast(
			HitLocation,
			HitNormal,
			BoneName,
			DamageMultiplier,
			TotalDamage,
			BaseDamage
		);
	}
}
