// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileExplosive.generated.h"

UCLASS()
class WEAPONSYSTEM_API AProjectileExplosive : public AProjectileBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionRadius = 500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "if true, damage not scaled based on distance from Origin."))
	bool bDoFullDamage = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> ExplosionChannel = ECC_Visibility;


public:
	AProjectileExplosive();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Explode();

	virtual void OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;
	
	virtual void OnProjectileLifeSpanEnd_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnExplosion();
};
