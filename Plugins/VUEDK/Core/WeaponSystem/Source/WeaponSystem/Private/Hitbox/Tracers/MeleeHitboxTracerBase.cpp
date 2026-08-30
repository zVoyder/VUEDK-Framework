// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/Tracers//MeleeHitboxTracerBase.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

UWorld* UMeleeHitboxTracerBase::GetWorld() const
{
	if (!Check())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxTracerBase::GetWorld: MeleeHitbox is not valid."));
		return nullptr;
	}
	
	UWorld* World = MeleeHitbox->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxTracerBase::GetWorld: World is not valid."));
		return nullptr;
	}
	
	return World;
}

void UMeleeHitboxTracerBase::Init(UMeleeHitbox* InMeleeHitbox)
{
	MeleeHitbox = InMeleeHitbox;
}

TArray<FHitResult> UMeleeHitboxTracerBase::TraceHitbox()
{
	TArray<FHitResult> HitResults;
	
	if (!Check())
		return HitResults;

	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return HitResults;
	
	World->SweepMultiByChannel(
		HitResults, 
		MeleeHitbox->GetComponentLocation(),
		MeleeHitbox->GetComponentLocation(),
		MeleeHitbox->GetComponentRotation().Quaternion(),
		GetCollisionChannel(),
		CreateCollisionShape(),
		CreateCollisionParams()
	);

#if !UE_BUILD_SHIPPING
	const AWeaponMelee* WeaponMelee = MeleeHitbox->GetWeaponMelee();
	const bool bDebug = FWeaponSystemModule::CVarMeleeTraceDebug.GetValueOnGameThread();
	if (bDebug && IsValid(WeaponMelee))
		DrawDebugHitboxTrace(WeaponMelee, MeleeHitbox->GetComponentTransform());
#endif

	return HitResults;
}

#if WITH_EDITOR
void UMeleeHitboxTracerBase::DrawHitboxPreview(const FTransform& HitboxTransform)
{
}
#endif

#if !UE_BUILD_SHIPPING
void UMeleeHitboxTracerBase::DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform)
{
}
#endif

FCollisionShape UMeleeHitboxTracerBase::CreateCollisionShape()
{
	return FCollisionShape();
}

FCollisionQueryParams UMeleeHitboxTracerBase::CreateCollisionParams() const
{
	FCollisionQueryParams Params;

	if (!Check())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxTracerBase::CreateCollisionParams: MeleeHitbox is not valid."));
		return Params;
	}
	
	Params.bTraceComplex = true;
	const AWeaponMelee* WeaponMelee = MeleeHitbox->GetWeaponMelee();
	if (!IsValid(WeaponMelee))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxTracerBase::CreateCollisionParams: WeaponMelee is not valid."));
		return Params;
	}
	
	Params.AddIgnoredActor(WeaponMelee->GetOwner());
	Params.AddIgnoredActor(WeaponMelee);
	Params.bTraceComplex = true;
	return Params;
}

ECollisionChannel UMeleeHitboxTracerBase::GetCollisionChannel() const
{
	if (!Check())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxTracerBase::GetCollisionChannel: MeleeHitbox is not valid."));
		return ECC_Visibility;
	}

	const AWeaponMelee* WeaponMelee = MeleeHitbox->GetWeaponMelee();
	if (!IsValid(WeaponMelee))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxTracerBase::GetCollisionChannel: WeaponMelee is not valid."));
		return ECC_Visibility;
	}
	
	return WeaponMelee->WeaponMeleeData.DamageHitBoxChannel;
}

bool UMeleeHitboxTracerBase::Check() const
{
	return IsValid(MeleeHitbox);
}
