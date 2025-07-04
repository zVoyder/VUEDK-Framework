// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ShooterBehaviourBase.h"
#include "Shooter/Projectiles/ProjectileBase.h"
#include "ShooterPhysicProjectileBehaviour.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProjectileSpawned,
	AProjectileBase*, Projectile
);

UCLASS()
class WEAPONSYSTEM_API UShooterPhysicProjectileBehaviour : public UShooterBehaviourBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnProjectileSpawned OnProjectileSpawned;

	// -- Projectiles --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooter|Projectile")
	FGameplayTag ProjectilesPoolTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Shooter|Projectile")
	float ProjectileSpeed = 1000.0f;

private:
	UPROPERTY()
	UActorPool* ProjectilePool;

public:
	UShooterPhysicProjectileBehaviour();

	virtual void Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel) override;

	UFUNCTION(BlueprintCallable)
	void SetProjectilesPool(const FGameplayTag InProjectilesPoolTag);

protected:
	virtual void OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& DirectionToTargetSpreaded) const override;

	virtual bool Check() const override;

private:
	void SpawnProjectile(const UShootPoint* ShootPoint, const FVector& DirectionToTarget) const;
};
