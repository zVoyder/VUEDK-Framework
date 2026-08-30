// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeleeHitboxTracerBase.h"
#include "Components/SphereComponent.h"
#include "MeleeHitboxSphereTracer.generated.h"

UCLASS()
class WEAPONSYSTEM_API UMeleeHitboxSphereTracer : public UMeleeHitboxTracerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float SphereRadius = 30.0f;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	USphereComponent* HitboxPreview;
#endif
	
public:
	UMeleeHitboxSphereTracer();

#if WITH_EDITOR
	virtual void DrawHitboxPreview(const FTransform& HitboxTransform) override;
#endif

#if !UE_BUILD_SHIPPING
	virtual void DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform) override;
#endif
	
protected:
	virtual FCollisionShape CreateCollisionShape() override;
};
