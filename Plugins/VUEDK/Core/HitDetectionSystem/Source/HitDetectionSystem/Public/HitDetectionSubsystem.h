// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HitDetectionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FHitDetectionEventData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	AActor* DamagedActor;
	UPROPERTY(BlueprintReadOnly)
	AActor* DamageCauser;
	UPROPERTY(BlueprintReadOnly)
	FVector HitLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector HitNormal;
	UPROPERTY(BlueprintReadOnly)
	FName BoneName;
	UPROPERTY(BlueprintReadOnly)
	float DamageMultiplier;
	UPROPERTY(BlueprintReadOnly)
	float TotalDamage;
	UPROPERTY(BlueprintReadOnly)
	float BaseDamage;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FHitDetectionDelegate,
	FHitDetectionEventData, HitEventData
);

UCLASS()
class HITDETECTIONSYSTEM_API UHitDetectionSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FHitDetectionDelegate OnPlayerPerformedHit;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void CallForHit(AController* InstigatedBy, AActor* DamagedActor, AActor* DamageCauser, const FVector& HitLocation, const FVector& HitNormal, const FName& BoneName, float DamageMultiplier, float TotalDamage, float BaseDamage) const;
};
