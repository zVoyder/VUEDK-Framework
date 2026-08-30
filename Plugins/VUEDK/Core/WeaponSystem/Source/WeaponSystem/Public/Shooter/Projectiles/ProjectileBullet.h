// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileBullet.generated.h"

UCLASS()
class WEAPONSYSTEM_API AProjectileBullet : public AProjectileBase
{
	GENERATED_BODY()

public:
	virtual void OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;
};
