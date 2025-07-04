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
	UFUNCTION(BlueprintCallable)
	void CallForHit(AController* InstigatedBy, const FVector& HitLocation, const FVector& HitNormal, const FName& BoneName, float DamageMultiplier, float TotalDamage, float BaseDamage) const;
};
