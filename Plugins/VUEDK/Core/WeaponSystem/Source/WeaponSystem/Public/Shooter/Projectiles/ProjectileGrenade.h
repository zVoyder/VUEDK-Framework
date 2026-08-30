// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileExplosive.h"
#include "ProjectileGrenade.generated.h"

UCLASS()
class WEAPONSYSTEM_API AProjectileGrenade : public AProjectileExplosive
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bExplodeOnHit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/Engine.ECollisionChannel", EditCondition = "bExplodeOnHit", EditConditionHides))
	int32 ExplodeOnHitChannelMask = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bExplodeOnHit", EditConditionHides))
	TArray<TSoftClassPtr<AActor>> ExplodeOnImpactActors;

public:
	AProjectileGrenade();
	
protected:
	virtual void OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;
	
	/**
	 * Checks if the projectile can explode on the given actor.
	 * @param Actor The actor to check against.
	 * @return True if the projectile can explode on the actor, false otherwise.
	 */
	bool CanExplodeOnActor(const AActor* Actor) const;
};
