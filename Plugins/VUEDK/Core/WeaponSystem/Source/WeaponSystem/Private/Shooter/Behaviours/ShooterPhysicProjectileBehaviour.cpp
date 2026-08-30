// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShooterPhysicProjectileBehaviour.h"
#include "WeaponSystem.h"
#include "Shooter/Shooter.h"
#include "Shooter/Projectiles/ProjectileBase.h"
#include "Utility/PoolsUtility.h"

UShooterPhysicProjectileBehaviour::UShooterPhysicProjectileBehaviour(): ProjectilePool(nullptr)
{
}

void UShooterPhysicProjectileBehaviour::Init(UShooter* InShooter)
{
	ProjectilePool = UPoolsUtility::GetPoolByTag(ProjectilesPoolTag);
	Super::Init(InShooter);
}

void UShooterPhysicProjectileBehaviour::SetProjectilesPool(const FGameplayTag InProjectilesPoolTag)
{
	UActorPool* Pool = UPoolsUtility::GetPoolByTag(InProjectilesPoolTag);
	ProjectilePool = IsValid(Pool) ? Pool : ProjectilePool;
}

void UShooterPhysicProjectileBehaviour::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& ShootDirection) const
{
	SpawnProjectile(ShootPoint, ShootDirection);
}

bool UShooterPhysicProjectileBehaviour::Check() const
{
	return Super::Check() && IsValid(ProjectilePool);
}

void UShooterPhysicProjectileBehaviour::SpawnProjectile(const UShootPoint* ShootPoint, const FVector& DirectionToTarget) const
{
	const FVector SpawnLocation = ShootPoint->GetShootPointLocation();
	AActor* ActorPrj = ProjectilePool->AcquireActorAtLocation(SpawnLocation);

	if (!IsValid(ActorPrj))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterPhysicProjectileBehaviour::OnDeployShoot_Implementation(), Invalid projectile actor from pool."));
		return;
	}

	AProjectileBase* Projectile = Cast<AProjectileBase>(ActorPrj);
	Projectile->Init(Shooter->GetOwner(), GetDamageTypeClass(), GetDamage(), GetMaxRange(), ProjectileSpeed, DirectionToTarget);
	OnProjectileSpawned.Broadcast(Projectile);
}
