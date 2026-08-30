// Copyright VUEDK, Inc. All Rights Reserved.

#include "Weapons/WeaponMelee.h"
#include "WeaponSystem.h"

AWeaponMelee::AWeaponMelee()
{
	MeleeMontagesManager = CreateDefaultSubobject<UMeleeMontagesManager>(TEXT("MeleeMontagesManager"));
	MeleeHitboxesManager = CreateDefaultSubobject<UMeleeHitboxesManager>(TEXT("MeleeHitboxesManager"));
}

UWeaponMontagesManagerBase* AWeaponMelee::GetMontagesManager() const
{
	return MeleeMontagesManager;
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
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::CanDefend: MeleeMontagesManager is not valid."));
		return false;
	}
	
	return !MeleeMontagesManager->IsBusy();
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
	MeleeMontagesManager->SetWantsToDefend(false);
	bIsBlockActive = false;
	return true;
}

void AWeaponMelee::EnableDamageHitbox() const
{
	MeleeHitboxesManager->SetTracingHitboxes(true);
}

void AWeaponMelee::DisableDamageHitbox() const
{
	MeleeHitboxesManager->SetTracingHitboxes(false);
}

void AWeaponMelee::SetBlockActive(const bool bActive)
{
	bIsBlockActive = bActive;
}

UWeaponMeleeAttackData* AWeaponMelee::GetCurrentAttack() const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::GetCurrentAttackMontageData: MeleeMontagesManager is not valid."));
		return nullptr;
	}

	return MeleeMontagesManager->GetCurrentAttackMontage();
}

void AWeaponMelee::SetAttackSpeedMultiplier(const float NewMultiplier) const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::SetAttackSpeedMultiplier: MeleeMontagesManager is not valid."));
		return;
	}

	MeleeMontagesManager->SetAttackSpeedMultiplier(NewMultiplier);
}

void AWeaponMelee::SetDefenseSpeedMultiplier(const float NewMultiplier) const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::SetDefenseSpeedMultiplier: MeleeMontagesManager is not valid."));
		return;
	}
	
	MeleeMontagesManager->SetDefenseSpeedMultiplier(NewMultiplier);
}

void AWeaponMelee::SetInterruptSpeedMultiplier(const float NewMultiplier) const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::SetInterruptSpeedMultiplier: MeleeMontagesManager is not valid."));
		return;
	}

	MeleeMontagesManager->SetInterruptSpeedMultiplier(NewMultiplier);
}

float AWeaponMelee::GetAttackSpeedMultiplier() const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::GetAttackSpeedMultiplier: MeleeMontagesManager is not valid."));
		return 1.0f;
	}
	
	return MeleeMontagesManager->GetAttackSpeedMultiplier();
}

float AWeaponMelee::GetDefenseSpeedMultiplier() const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::GetDefenseSpeedMultiplier: MeleeMontagesManager is not valid."));
		return 1.0f;
	}

	return MeleeMontagesManager->GetDefenseSpeedMultiplier();
}

float AWeaponMelee::GetInterruptSpeedMultiplier() const
{
	if (!IsValid(MeleeMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::GetInterruptSpeedMultiplier: MeleeMontagesManager is not valid."));
		return 1.0f;
	}

	return MeleeMontagesManager->GetInterruptSpeedMultiplier();
}

void AWeaponMelee::CallComboStartedEvent(UWeaponMeleeAttackData* AttackData)
{
	OnWeaponComboStart.Broadcast(AttackData);
	OnComboStarted(AttackData);
}

void AWeaponMelee::CallComboEndedEvent(UWeaponMeleeAttackData* AttackData)
{
	OnWeaponComboEnd.Broadcast(AttackData);
	OnComboEnded(AttackData);
}

void AWeaponMelee::CallAttackStartedEvent(UWeaponMeleeAttackData* AttackData, int32 AttackIndex)
{
	OnWeaponAttackStart.Broadcast(AttackData, AttackIndex);
	OnAttackStarted(AttackData, AttackIndex);
}

void AWeaponMelee::CallPreHitEvent(UMeleeHitbox* Hitbox, const FHitResult& HitResult, float Damage)
{
	OnWeaponAttackPreHit.Broadcast(Hitbox, HitResult, Damage);
	OnAttackPreHit(Hitbox, HitResult, Damage);
}

void AWeaponMelee::CallHitEvent(UMeleeHitbox* Hitbox, const FHitResult& HitResult, float Damage)
{
	OnWeaponAttackHit.Broadcast(Hitbox, HitResult, Damage);
	OnAttackHit(Hitbox, HitResult, Damage);
}

void AWeaponMelee::CallInterruptEvent(UMeleeHitbox* Hitbox, const FHitResult& HitResult)
{
	OnWeaponAttackInterrupt.Broadcast(Hitbox, HitResult);
	OnAttackInterrupted(Hitbox, HitResult);
}

void AWeaponMelee::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(MeleeHitboxesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponMelee::BeginPlay: MeleeHitboxesManager is not valid."));
		return;
	}
	
	MeleeHitboxesManager->Init(this);
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
	
	return !MeleeMontagesManager->IsBusy();
}

void AWeaponMelee::OnAttackPreHit_Implementation(UMeleeHitbox* Hitbox, FHitResult HitResult, float Damage)
{
}

void AWeaponMelee::OnComboStarted_Implementation(UWeaponMeleeAttackData* AttackData)
{
}

void AWeaponMelee::OnComboEnded_Implementation(UWeaponMeleeAttackData* AttackData)
{
}

void AWeaponMelee::OnAttackStarted_Implementation(UWeaponMeleeAttackData* AttackData, int32 AttackIndex)
{
}

void AWeaponMelee::OnAttackHit_Implementation(UMeleeHitbox* Hitbox, FHitResult HitResult, float Damage)
{
}

void AWeaponMelee::OnAttackInterrupted_Implementation(UMeleeHitbox* Hitbox, FHitResult HitResult)
{
}
