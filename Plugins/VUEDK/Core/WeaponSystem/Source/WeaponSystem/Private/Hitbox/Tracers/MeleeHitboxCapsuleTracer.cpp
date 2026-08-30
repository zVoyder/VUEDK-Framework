// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/Tracers/MeleeHitboxCapsuleTracer.h"

#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

UMeleeHitboxCapsuleTracer::UMeleeHitboxCapsuleTracer()
{
#if WITH_EDITORONLY_DATA
	HitboxPreview = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitboxPreview"));
	HitboxPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#endif
}

#if WITH_EDITOR
void UMeleeHitboxCapsuleTracer::DrawHitboxPreview(const FTransform& HitboxTransform)
{
	Super::DrawHitboxPreview(HitboxTransform);

	if (HitboxPreview)
	{
		HitboxPreview->SetWorldLocation(HitboxTransform.GetLocation());
		HitboxPreview->SetWorldRotation(HitboxTransform.GetRotation());
		HitboxPreview->SetCapsuleHalfHeight(CapsuleHalfHeight);
		HitboxPreview->SetCapsuleRadius(CapsuleRadius);
		HitboxPreview->ShapeColor = HitboxColor;
	}
}
#endif

#if !UE_BUILD_SHIPPING
void UMeleeHitboxCapsuleTracer::DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform)
{
	Super::DrawDebugHitboxTrace(WeaponMelee, HitboxTransform);

	DrawDebugCapsule(
		GetWorld(),
		HitboxTransform.GetLocation(),
		CapsuleHalfHeight,
		CapsuleRadius,
		HitboxTransform.GetRotation(),
		HitboxColor,
		false,
		FWeaponSystemModule::CVarMeleeTraceDebugDuration.GetValueOnGameThread()
	);
}
#endif

FCollisionShape UMeleeHitboxCapsuleTracer::CreateCollisionShape()
{
	return FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
}
