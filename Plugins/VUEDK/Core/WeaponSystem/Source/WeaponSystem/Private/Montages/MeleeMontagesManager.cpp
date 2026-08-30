// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/MeleeMontagesManager.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

UMeleeMontagesManager::UMeleeMontagesManager() : WeaponMelee(nullptr)
{
}

void UMeleeMontagesManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ProcessDefense();
}

bool UMeleeMontagesManager::IsBusy_Implementation() const
{
	return Super::IsBusy_Implementation() || IsMontageDefending() || IsMontageInterrupting() || IsMontageAttacking();
}

void UMeleeMontagesManager::SetAttackSpeedMultiplier(const float Multiplier)
{
	AttackSpeedMultiplier = FMath::Clamp(Multiplier, 0.0f, 1.0f);
}

void UMeleeMontagesManager::SetDefenseSpeedMultiplier(const float Multiplier)
{
	DefenseSpeedMultiplier = FMath::Clamp(Multiplier, 0.0f, 1.0f);
}

void UMeleeMontagesManager::SetInterruptSpeedMultiplier(const float Multiplier)
{
	InterruptSpeedMultiplier = FMath::Clamp(Multiplier, 0.0f, 1.0f);
}

float UMeleeMontagesManager::GetAttackSpeedMultiplier() const
{
	return AttackSpeedMultiplier;
}

float UMeleeMontagesManager::GetDefenseSpeedMultiplier() const
{
	return DefenseSpeedMultiplier;
}

float UMeleeMontagesManager::GetInterruptSpeedMultiplier() const
{
	return InterruptSpeedMultiplier;
}

float UMeleeMontagesManager::GetAttackSpeed() const
{
	return BaseAttackSpeed * AttackSpeedMultiplier;
}

float UMeleeMontagesManager::GetDefenseSpeed() const
{
	return BaseDefenseSpeed * DefenseSpeedMultiplier;
}

float UMeleeMontagesManager::GetInterruptSpeed() const
{
	return BaseInterruptSpeed * InterruptSpeedMultiplier;
}

int32 UMeleeMontagesManager::GetAttackIndex() const
{
	return CurrentAttackIndex;
}

UWeaponMeleeAttackData* UMeleeMontagesManager::GetCurrentAttackMontage() const
{
	return CurrentAttack;
}

bool UMeleeMontagesManager::IsMontageInterrupting() const
{
	return bIsInterrupting;
}

bool UMeleeMontagesManager::IsMontageDefending() const
{
	return bIsDefending;
}

bool UMeleeMontagesManager::IsMontageAttacking() const
{
	return bIsAttacking;
}

void UMeleeMontagesManager::SetWantsToDefend(const bool bWants)
{
	bWantsToDefend = bWants;
}

void UMeleeMontagesManager::CheckBufferAttack()
{
	if (bWantsToAttack)
		bHasBufferedAttack = true;
}

void UMeleeMontagesManager::ClearBufferedAttack()
{
	bHasBufferedAttack = false;
}

void UMeleeMontagesManager::OnAttackBeginNotify()
{
	ClearBufferedAttack();
}

void UMeleeMontagesManager::OnAttackFinishedNotify()
{
	if (bIsInterrupting)
		return;

	if (bWantsToAttack || bHasBufferedAttack)
	{
		bEndAttack = false;
		PlayNextAttackMontage();
		return;
	}

	EndAttackSequence();
}

void UMeleeMontagesManager::SetupWeapons(AWeaponBase* InWeapon)
{
	Super::SetupWeapons(InWeapon);
	WeaponMelee = Cast<AWeaponMelee>(InWeapon);
}

void UMeleeMontagesManager::BindEvents()
{
	Super::BindEvents();
	WeaponMelee->OnWeaponAttackInterrupt.AddDynamic(this, &UMeleeMontagesManager::OnWeaponAttackInterrupted);
}

void UMeleeMontagesManager::UnbindEvents()
{
	Super::UnbindEvents();
	WeaponMelee->OnWeaponAttackInterrupt.RemoveDynamic(this, &UMeleeMontagesManager::OnWeaponAttackInterrupted);
}

void UMeleeMontagesManager::OnWeaponAttackSuccess()
{
	bWantsToAttack = true;
	bEndAttack = false;

	if (!bIsAttacking && bWantsToAttack)
	{
		StartComboAttack();
		bIsAttacking = true;
	}
}

void UMeleeMontagesManager::OnWeaponEndAttack()
{
	Super::OnWeaponEndAttack();
	bWantsToAttack = false;
	bEndAttack = true;
}

void UMeleeMontagesManager::OnWeaponAttackInterrupted(UMeleeHitbox* Hitbox, FHitResult HitResult)
{
	if (CurrentAttack == nullptr)
		return;

	if (!IsValid(CurrentAttack->AttackInterruptMontage.GetCharacterMontage()))
		return;

	UWeaponMeleeAttackData* WeaponMontage = CurrentAttack;
	bIsInterrupting = true;
	EndAttackSequence();
	WeaponMontage->AttackInterruptMontage.OnWeaponMontageFinished.AddUniqueDynamic(this, &UMeleeMontagesManager::OnAttackInterruptFinished);
	StartWeaponMontage(
		WeaponMontage->AttackInterruptMontage,
		GetInterruptSpeed(),
		GetInterruptSpeed()
	);
}

void UMeleeMontagesManager::OnAttackInterruptFinished(bool bInterrupted)
{
	bIsInterrupting = false;
}

void UMeleeMontagesManager::StartComboAttack()
{
	if (Attacks.IsEmpty())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeMontagesManager::StartComboAttack: %s in %s has no attack montages."), *GetName(), *GetOwner()->GetName());
		return;
	}

	ResetComboAttack();
	UWeaponMeleeAttackData* Attack = Attacks[0];

	if (!IsValid(Attack))
		return;

	FAlphaBlendArgs WeaponBlendArgs;
	if (IsValid(DefensiveMontage.GetWeaponMontage()))
		WeaponBlendArgs = DefensiveMontage.GetWeaponMontage()->BlendOut;

	CurrentAttack = Attack;
	StartWeaponMontageWithBlends(
		Attack->AttackMontage,
		GetAttackSpeed(),
		GetAttackSpeed(),
		WeaponBlendArgs,
		StartBlendIn
	);

	WeaponMelee->CallComboStartedEvent(Attack);
	WeaponMelee->CallAttackStartedEvent(Attack, CurrentAttackIndex);
}

void UMeleeMontagesManager::EndComboAttack()
{
	if (CurrentAttack == nullptr || !IsValid(CurrentAttack->AttackMontage.GetCharacterMontage()))
		return;

	FAlphaBlendArgs WeaponBlendArgs;
	if (IsValid(DefensiveMontage.GetWeaponMontage()))
		WeaponBlendArgs = DefensiveMontage.GetWeaponMontage()->BlendOut;

	StopWeaponMontageWithBlends(
		CurrentAttack->AttackMontage,
		WeaponBlendArgs,
		StopBlendOut
	);

	WeaponMelee->CallComboEndedEvent(CurrentAttack);
}

void UMeleeMontagesManager::ResetComboAttack()
{
	CurrentAttackIndex = 0;
	bIsAttacking = false;
	bEndAttack = false;
}

void UMeleeMontagesManager::PlayNextAttackMontage()
{
	CurrentAttackIndex++;
	CurrentAttackIndex %= Attacks.Num();
	PlayAttackMontageAt(CurrentAttackIndex);
}

void UMeleeMontagesManager::PlayAttackMontageAt(const int32 AttackIndex)
{
	if (!Attacks.IsValidIndex(AttackIndex))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UMeleeMontagesManager::PlayAttackMontageAt: Invalid index %d for attack montage in %s."), AttackIndex, *GetName());
		return;
	}

	UWeaponMeleeAttackData* AttackMontage = Attacks[AttackIndex];
	PlayAttackMontage(AttackMontage);
}

void UMeleeMontagesManager::PlayAttackMontage(UWeaponMeleeAttackData* Attack)
{
	CurrentAttack = Attack;

	StartWeaponMontage(
		Attack->AttackMontage,
		GetAttackSpeed(),
		GetAttackSpeed()
	);

	WeaponMelee->CallAttackStartedEvent(Attack, CurrentAttackIndex);
}

void UMeleeMontagesManager::EndAttackSequence()
{
	EndComboAttack();
	ResetComboAttack();
	CurrentAttack = nullptr;
}

void UMeleeMontagesManager::PlayDefensiveMontage()
{
	if (bIsDefenseInCooldown)
		return;

	if (!IsValid(DefensiveMontage.GetCharacterMontage()))
		return;
	
	bIsDefending = true;
	StartWeaponMontage(
		DefensiveMontage,
		GetDefenseSpeed(),
		GetDefenseSpeed()
	);

	StartDefenseCooldownTimer();
}

void UMeleeMontagesManager::StopDefensiveMontage()
{
	if (!IsMontageDefending())
		return;
	
	if (!IsValid(DefensiveMontage.GetCharacterMontage()))
		return;

	bIsDefending = false;
	FAlphaBlendArgs WeaponBlendArgs;
	if (IsValid(DefensiveMontage.GetWeaponMontage()))
		WeaponBlendArgs = DefensiveMontage.GetWeaponMontage()->BlendOut;

	StopWeaponMontageWithBlends(
		DefensiveMontage,
		WeaponBlendArgs,
		DefensiveMontage.GetCharacterMontage()->BlendOut
	);
}

void UMeleeMontagesManager::ProcessDefense()
{
	if (bWantsToDefend)
		PlayDefensiveMontage();
	else
		StopDefensiveMontage();
}

void UMeleeMontagesManager::StartDefenseCooldownTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeMontagesManager::StartDefenseCooldownTimer: World is not valid."));
		return;
	}

	bIsDefenseInCooldown = true;
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(
		DefenseCooldownTimer,
		this,
		&UMeleeMontagesManager::ResetDefenseCooldown,
		DefenseCooldown,
		false
	);
}

void UMeleeMontagesManager::StopDefenseCooldownTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeMontagesManager::StopDefensiveMontage: World is not valid."));
		return;
	}

	bIsDefenseInCooldown = false;
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(DefenseCooldownTimer);
}

void UMeleeMontagesManager::ResetDefenseCooldown()
{
	bIsDefenseInCooldown = false;
}
