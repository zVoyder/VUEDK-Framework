// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Projectiles/ProjectileExplosive.h"
#include "Kismet/GameplayStatics.h"

AProjectileExplosive::AProjectileExplosive()
{
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectileExplosive::Explode()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	AController* InstigatorController = IsValid(ProjectileInstigator) ? ProjectileInstigator->GetInstigatorController() : GetInstigatorController();
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		GetDamage(),
		GetActorLocation(),
		ExplosionRadius,
		DamageTypeClass,
		IgnoredActors,
		this,
		InstigatorController,
		bDoFullDamage,
		ExplosionChannel
	);

	OnExplosion();
	DisposeProjectile();
}

void AProjectileExplosive::OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	Explode();
}

void AProjectileExplosive::OnProjectileLifeSpanEnd_Implementation()
{
	Explode();
}

void AProjectileExplosive::OnExplosion_Implementation()
{
}
