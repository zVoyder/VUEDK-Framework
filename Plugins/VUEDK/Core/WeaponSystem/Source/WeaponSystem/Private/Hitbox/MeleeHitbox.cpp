// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/MeleeHitbox.h"
#include "WeaponSystem.h"
#include "Hitbox/Tracers/MeleeHitboxCapsuleTracer.h"
#include "Hitbox/Tracers/MeleeHitboxCubeTracer.h"
#include "Hitbox/Tracers/MeleeHitboxSphereTracer.h"
#include "Hitbox/Tracers/MeleeHitboxTracerBase.h"
#include "Weapons/WeaponMelee.h"

UMeleeHitbox::UMeleeHitbox()
{
	PrimaryComponentTick.bCanEverTick = true;
	DamageProcessor = CreateDefaultSubobject<UMeleeHitboxDamageProcessor>(TEXT("DamageProcessor"));
}

void UMeleeHitbox::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITOR
	if (IsValid(HitboxTracer))
	{
		HitboxTracer->DrawHitboxPreview(GetComponentTransform());
	}
#endif
}

void UMeleeHitbox::Init(AWeaponMelee* InWeaponMelee)
{
	if (!IsValid(InWeaponMelee))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitbox::Init: InWeaponMelee is not valid."));
		return;
	}

	if (!IsValid(HitboxTracer))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitbox::Init: HitboxType is not valid."));
		return;
	}
	
	HitboxTracer->Init(this);
	WeaponMelee = InWeaponMelee;
}

TArray<FHitResult> UMeleeHitbox::TraceHitbox() const
{
	if (!Check())
		return TArray<FHitResult>();

	return HitboxTracer->TraceHitbox();
}

float UMeleeHitbox::GetProcessedDamage()
{
	if (!Check())
		return 0.0f;

	return DamageProcessor->ProcessDamageNative(WeaponMelee->GetWeaponDamage(), this, GetWeaponMelee());
}

TSubclassOf<UDamageType> UMeleeHitbox::GetDamageType() const
{
	if (!Check())
		return UDamageType::StaticClass();

	if (DamageProcessor->bUseCustomDamageType)
		return DamageProcessor->GetDamageType();
	
	return WeaponMelee->WeaponMeleeData.DamageTypeClass;
}

AWeaponMelee* UMeleeHitbox::GetWeaponMelee() const
{
	return WeaponMelee;
}

bool UMeleeHitbox::Check() const
{
	return IsValid(WeaponMelee) && IsValid(HitboxTracer) && IsValid(DamageProcessor);
}
