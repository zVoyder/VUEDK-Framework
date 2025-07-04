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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ECollisionChannel", EditCondition = "bExplodeOnHit", EditConditionHides))
	int32 ExplodeOnHitChannelMask = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bExplodeOnHit", EditConditionHides))
	TArray<TSoftClassPtr<AActor>> ExplodeOnImpactActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeToExplosion = 5.0f;

public:
	AProjectileGrenade();
	
protected:
	float CalculateLifeSpan_Implementation(const float InRange, const float InSpeed) const override;
	
	virtual void OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;
	
	bool CanExplodeOnActor(const AActor* Actor) const;
};
