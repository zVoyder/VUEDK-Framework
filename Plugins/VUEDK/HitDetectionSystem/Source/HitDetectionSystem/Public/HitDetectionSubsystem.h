// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HitDetectionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FHitDetectionDelegate,
	FVector, HitLocation,
	FVector, HitNormal,
	FName, BoneName,
	float, DamageMultiplier,
	float, TotalDamage,
	float, BaseDamage
);

UCLASS()
class HITDETECTIONSYSTEM_API UHitDetectionSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FHitDetectionDelegate OnPlayerPerformedHit;

public:
	/**
	 * Calls the hit detection subsystem to handle a hit event.
	 * @param InstigatedBy The controller that instigated the hit.
	 * @param HitLocation The location of the hit.
	 * @param HitNormal The normal vector at the hit location.
	 * @param BoneName The name of the bone that was hit.
	 * @param DamageMultiplier The multiplier applied to the damage.
	 * @param TotalDamage The total damage dealt by the hit.
	 * @param BaseDamage The base damage value before any multipliers.
	 */
	UFUNCTION(BlueprintCallable)
	void CallForHit(AController* InstigatedBy, const FVector& HitLocation, const FVector& HitNormal, const FName& BoneName, float DamageMultiplier, float TotalDamage, float BaseDamage) const;
};
