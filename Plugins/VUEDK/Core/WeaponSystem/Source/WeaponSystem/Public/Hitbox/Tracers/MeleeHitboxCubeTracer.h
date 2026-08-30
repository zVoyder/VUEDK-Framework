// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeleeHitboxTracerBase.h"
#include "Components/BoxComponent.h"
#include "MeleeHitboxCubeTracer.generated.h"

UCLASS()
class WEAPONSYSTEM_API UMeleeHitboxCubeTracer : public UMeleeHitboxTracerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	FVector BoxHalfExtent = FVector(10.f, 10.f, 10.f);

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBoxComponent* HitboxPreview;
#endif

public:
	UMeleeHitboxCubeTracer();

#if WITH_EDITOR
	virtual void DrawHitboxPreview(const FTransform& HitboxTransform) override;
#endif

#if !UE_BUILD_SHIPPING
	virtual void DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform) override;
#endif
	
protected:
	virtual FCollisionShape CreateCollisionShape() override;
};
