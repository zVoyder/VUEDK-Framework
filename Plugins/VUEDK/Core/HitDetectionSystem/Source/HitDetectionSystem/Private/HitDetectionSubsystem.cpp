// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitDetectionSubsystem.h"
#include "HitDetectionSystem.h"

void UHitDetectionSubsystem::CallForHit(AController* InstigatedBy, AActor* DamagedActor, AActor* DamageCauser, const FVector& HitLocation, const FVector& HitNormal, const FName& BoneName, float DamageMultiplier, float TotalDamage, float BaseDamage) const
{
	const UWorld* World = GetWorld();

	if (!World || !InstigatedBy)
	{
		UE_LOG(LogHitDetectionSystem, Warning, TEXT("HitDetectionSubsystem::CallForHit: Invalid parameters."));
		return;
	}

	if (InstigatedBy == GetLocalPlayer()->GetPlayerController(World))
	{
		FHitDetectionEventData HitEventData;
		HitEventData.DamagedActor = DamagedActor;
		HitEventData.DamageCauser = DamageCauser;
		HitEventData.HitLocation = HitLocation;
		HitEventData.HitNormal = HitNormal;
		HitEventData.BoneName = BoneName;
		HitEventData.DamageMultiplier = DamageMultiplier;
		HitEventData.TotalDamage = TotalDamage;
		HitEventData.BaseDamage = BaseDamage;
		OnPlayerPerformedHit.Broadcast(HitEventData);
	}
}
