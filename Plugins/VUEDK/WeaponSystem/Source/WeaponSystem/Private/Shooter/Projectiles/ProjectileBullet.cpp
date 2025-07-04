// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Projectiles/ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"

void AProjectileBullet::OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
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
