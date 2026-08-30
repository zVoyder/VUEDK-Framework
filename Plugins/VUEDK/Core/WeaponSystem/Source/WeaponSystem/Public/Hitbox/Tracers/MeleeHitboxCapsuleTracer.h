// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeleeHitboxTracerBase.h"
#include "Components/CapsuleComponent.h"
#include "MeleeHitboxCapsuleTracer.generated.h"

UCLASS()
class WEAPONSYSTEM_API UMeleeHitboxCapsuleTracer : public UMeleeHitboxTracerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float CapsuleHalfHeight = 44.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float CapsuleRadius = 22.0f;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UCapsuleComponent* HitboxPreview;
#endif

public:
	UMeleeHitboxCapsuleTracer();
	
#if WITH_EDITOR
	virtual void DrawHitboxPreview(const FTransform& HitboxTransform) override;
#endif
	
#if !UE_BUILD_SHIPPING
	virtual void DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform) override;
#endif

protected:
	virtual FCollisionShape CreateCollisionShape() override;
};
