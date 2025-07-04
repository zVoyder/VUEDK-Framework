// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/HitZoneRedirectorMode.h"
#include "UObject/Object.h"
#include "HitZoneHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnZoneHitAnyDamage,
	float, BaseDamage,
	float, TotalDamage,
	const class UDamageType*, DamageType,
	AController*, InstigatedBy,
	AActor*, DamageCauser
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(
	FOnZoneHitPointDamage,
	float, BaseDamage,
	float, TotalDamage,
	FVector, HitLocation,
	FVector, HitNormal,
	FName, BoneName,
	const class UDamageType*, DamageType,
	AController*, InstigatedBy,
	AActor*, DamageCauser
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(
	FOnZoneHitRadialDamage,
	float, BaseDamage,
	float, TotalDamage,
	const class UDamageType*, DamageType,
	FVector, Origin,
	const FHitResult&, HitInfo,
	AController*, InstigatedBy,
	AActor*, DamageCauser
);

UCLASS(EditInlineNew)
class HITDETECTIONSYSTEM_API UHitZoneHandler : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnZoneHitAnyDamage OnZoneHitAnyDamage;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnZoneHitPointDamage OnZoneHitPointDamage;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnZoneHitRadialDamage OnZoneHitRadialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitZone")
	bool bReceiveRadialDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitZone")
	float Multiplier = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HitZone")
	EHitZoneRedirectorMode BoneRedirectorMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HitZone", meta = (EditCondition = "BoneRedirectorMode == EHitZoneRedirectorMode::CustomName", EditConditionHides))
	FName BoneRedirector = NAME_None;

private:
	UPROPERTY()
	AActor* Owner;
	UPROPERTY()
	UPrimitiveComponent* HitBox;

public:
	/**
	 * Initializes the hit zone handler with the specified owner and hit box.
	 * @param InOwner The actor that owns this hit zone handler.
	 * @param InHitBox The primitive component representing the hit box.
	 */
	void Init(AActor* InOwner, UPrimitiveComponent* InHitBox);

private:
	/**
	 * Checks the validity or state of the hit zone handler.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;
	
	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	/**
	 * Returns the bone name associated with this hit zone.
	 * @return The bone name as FName.
	 */
	FName GetBoneName() const;
};
