// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/Tracers/MeleeHitboxSphereTracer.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

UMeleeHitboxSphereTracer::UMeleeHitboxSphereTracer()
{
#if WITH_EDITORONLY_DATA
	HitboxPreview = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	HitboxPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#endif
}

#if WITH_EDITOR
void UMeleeHitboxSphereTracer::DrawHitboxPreview(const FTransform& HitboxTransform)
{
	Super::DrawHitboxPreview(HitboxTransform);

	if (HitboxPreview)
	{
		HitboxPreview->SetWorldLocation(HitboxTransform.GetLocation());
		HitboxPreview->SetWorldRotation(HitboxTransform.GetRotation());
		HitboxPreview->SetSphereRadius(SphereRadius);
		HitboxPreview->ShapeColor = HitboxColor;
	}
}
#endif

#if !UE_BUILD_SHIPPING
void UMeleeHitboxSphereTracer::DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform)
{
	Super::DrawDebugHitboxTrace(WeaponMelee, HitboxTransform);

	DrawDebugSphere(
		GetWorld(),
		HitboxTransform.GetLocation(),
		SphereRadius,
		12,
		HitboxColor,
		false,
		FWeaponSystemModule::CVarMeleeTraceDebugDuration.GetValueOnGameThread()
	);
}
#endif

FCollisionShape UMeleeHitboxSphereTracer::CreateCollisionShape()
{
	return FCollisionShape::MakeSphere(SphereRadius);
}
