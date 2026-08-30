// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Projectiles/ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"

void AProjectileBullet::OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	UGameplayStatics::ApplyPointDamage(
		ImpactResult.GetActor(),
		GetDamage(),
		ImpactResult.ImpactNormal,
		ImpactResult,
		IsValid(ProjectileInstigator) ? ProjectileInstigator->GetInstigatorController() : GetInstigatorController(),
		this,
		DamageTypeClass
	);

	DisposeProjectile();
}
