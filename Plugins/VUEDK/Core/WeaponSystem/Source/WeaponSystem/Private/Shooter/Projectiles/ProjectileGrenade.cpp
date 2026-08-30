// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Projectiles/ProjectileGrenade.h"

AProjectileGrenade::AProjectileGrenade(): bExplodeOnHit(false)
{
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
}

void AProjectileGrenade::OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (CanExplodeOnActor(ImpactResult.GetActor()))
		Explode();
}

bool AProjectileGrenade::CanExplodeOnActor(const AActor* Actor) const
{
	if (!IsValid(Actor) || !bExplodeOnHit)
		return false;

	const USceneComponent* ActorComponent = Actor->GetRootComponent();
	if (IsValid(ActorComponent))
	{
		const ECollisionChannel ActorChannel = ActorComponent->GetCollisionObjectType();
		if ((ExplodeOnHitChannelMask & (1 << ActorChannel)) != 0)
			return true;
	}
	
	return ExplodeOnImpactActors.ContainsByPredicate([Actor](const TSoftClassPtr<AActor>& ClassPtr)
	{
		return ClassPtr.IsValid() && Actor->IsA(ClassPtr.Get());
	});
}
