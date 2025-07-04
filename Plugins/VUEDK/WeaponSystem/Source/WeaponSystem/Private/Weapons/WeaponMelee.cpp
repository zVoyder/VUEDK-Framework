// Copyright VUEDK, Inc. All Rights Reserved.

#include "Weapons/WeaponMelee.h"
#include "WeaponSystem.h"
#include "Kismet/GameplayStatics.h"

AWeaponMelee::AWeaponMelee()
{
	MeleeMontagesManager = CreateDefaultSubobject<UMeleeMontagesManager>(TEXT("MeleeMontagesManager"));
	DamageHitboxPreview = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageHitboxPreview"));
	DamageHitboxPreview->SetupAttachment(WeaponMesh);
	DamageHitboxPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponMelee::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceDamageHitbox();
}

bool AWeaponMelee::IsWeaponAttacking() const
{
	return MeleeMontagesManager->IsMontageAttacking();
}

bool AWeaponMelee::IsWeaponBlocking() const
{
	return MeleeMontagesManager->IsMontageDefending() && bIsBlockActive;
}

bool AWeaponMelee::CanDefend_Implementation() const
{
	return IsValid(MeleeMontagesManager->DefensiveMontage.GetCharacterMontage());
}

bool AWeaponMelee::StartDefense() const
{
	if (!CanDefend())
        return false;
	
	MeleeMontagesManager->SetWantsToDefend(true);
	return true;
}

bool AWeaponMelee::StopDefense()
{
	if (!CanDefend())
		return false;
	
	MeleeMontagesManager->SetWantsToDefend(false);
	bIsBlockActive = false;
	return true;
}

void AWeaponMelee::EnableDamageHitbox()
{
	if (bIsHitboxEnabled)
		return;

	bIsHitboxEnabled = true;
	ClearHitActors();
}

void AWeaponMelee::DisableDamageHitbox()
{
	if (!bIsHitboxEnabled)
		return;

	bIsHitboxEnabled = false;
}

void AWeaponMelee::SetBlockActive(const bool bActive)
{
	bIsBlockActive = bActive;
}

bool AWeaponMelee::NativeDeployWeaponAttack()
{
	if (!Super::NativeDeployWeaponAttack())
		return false;
	
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::NativeDeployWeaponAttack: MeleeMontagesManager is not valid."));
		return false;
	}
	
	if (MeleeMontagesManager->IsBusy())
		return false;

	return true;
}

void AWeaponMelee::OnWeaponAttackInterrupted_Implementation()
{
}

void AWeaponMelee::TraceDamageHitbox()
{
	if (!bIsHitboxEnabled)
		return;

	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::TraceHitbox: World is not valid."));
		return;
	}

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(this);

	World->SweepMultiByChannel(
		HitResults,
		DamageHitboxPreview->GetComponentLocation(),
		DamageHitboxPreview->GetComponentLocation(),
		DamageHitboxPreview->GetComponentRotation().Quaternion(),
		WeaponMeleeData.DamageHitBoxChannel,
		FCollisionShape::MakeCapsule(DamageHitboxPreview->GetScaledCapsuleRadius(), DamageHitboxPreview->GetScaledCapsuleHalfHeight()),
		Params
	);

	HandleHitActors(HitResults);

#if WITH_EDITORONLY_DATA
	if (bDebug)
	{
		DrawDebugCapsule(
			World,
			DamageHitboxPreview->GetComponentLocation(),
			DamageHitboxPreview->GetScaledCapsuleHalfHeight(),
			DamageHitboxPreview->GetScaledCapsuleRadius(),
			DamageHitboxPreview->GetComponentRotation().Quaternion(),
			ActorsCurrentlyInHitbox.IsEmpty() ? FColor::Green : FColor::Red,
			false,
			0.1f
		);
	}
#endif
}

void AWeaponMelee::HandleHitActors(TArray<FHitResult> HitResults)
{
	TSet<AActor*> NewActorsInHitbox;
	
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor) || NewActorsInHitbox.Contains(HitActor))
			continue;

		if (WeaponMeleeData.AttackInterruptors.Contains(HitResult.Component->GetCollisionObjectType()))
		{
			OnWeaponAttackInterrupt.Broadcast();
			OnWeaponAttackInterrupted();
			return;
		}

		NewActorsInHitbox.Add(HitActor);
		if (!ActorsCurrentlyInHitbox.Contains(HitActor))
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				GetWeaponDamage(),
				HitResult.ImpactNormal,
				HitResult,
				GetInstigatorController(),
				GetOwner(),
				WeaponMeleeData.DamageTypeClass
			);
		}
	}
	
	for (const AActor* PrevActor : ActorsCurrentlyInHitbox)
	{
		if (!NewActorsInHitbox.Contains(PrevActor))
		{
			// Actor was in hitbox but is no longer in it, handle any logic here if needed
		}
	}

	ActorsCurrentlyInHitbox = NewActorsInHitbox;
}

void AWeaponMelee::ClearHitActors()
{
	ActorsCurrentlyInHitbox.Empty();
}
