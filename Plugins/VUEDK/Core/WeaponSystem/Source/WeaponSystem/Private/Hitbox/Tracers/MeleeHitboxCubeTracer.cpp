// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/Tracers/MeleeHitboxCubeTracer.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

UMeleeHitboxCubeTracer::UMeleeHitboxCubeTracer()
{
#if WITH_EDITORONLY_DATA
	HitboxPreview = CreateDefaultSubobject<UBoxComponent>(TEXT("HitboxPreview"));
	HitboxPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#endif
}

#if WITH_EDITOR
void UMeleeHitboxCubeTracer::DrawHitboxPreview(const FTransform& HitboxTransform)
{
	Super::DrawHitboxPreview(HitboxTransform);

	if (HitboxPreview)
	{
		HitboxPreview->SetWorldLocation(HitboxTransform.GetLocation());
		HitboxPreview->SetWorldRotation(HitboxTransform.GetRotation());
		HitboxPreview->SetBoxExtent(BoxHalfExtent);
		HitboxPreview->ShapeColor = HitboxColor;
	}
}
#endif

#if !UE_BUILD_SHIPPING
void UMeleeHitboxCubeTracer::DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform)
{
	Super::DrawDebugHitboxTrace(WeaponMelee, HitboxTransform);

	DrawDebugBox(
		GetWorld(),
		HitboxTransform.GetLocation(),
		BoxHalfExtent,
		HitboxTransform.GetRotation(),
		HitboxColor,
		false,
		FWeaponSystemModule::CVarMeleeTraceDebugDuration.GetValueOnGameThread()
	);
}
#endif

FCollisionShape UMeleeHitboxCubeTracer::CreateCollisionShape()
{
	return FCollisionShape::MakeBox(BoxHalfExtent);
}
