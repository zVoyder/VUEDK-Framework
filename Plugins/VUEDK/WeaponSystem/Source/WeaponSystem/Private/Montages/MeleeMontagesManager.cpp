// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/MeleeMontagesManager.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

UMeleeMontagesManager::UMeleeMontagesManager(): WeaponMelee(nullptr)
{
}

void UMeleeMontagesManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ProcessDefense();
}

bool UMeleeMontagesManager::IsBusy_Implementation() const
{
	return Super::IsBusy_Implementation() || IsMontageDefending() || IsMontageInterrupting();
}

int32 UMeleeMontagesManager::GetAttackIndex() const
{
	return CurrentAttackIndex;
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

void UMeleeMontagesManager::BeginPlay()
{
	Super::BeginPlay();

	if (AttackMontages.IsEmpty())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeMontagesManager::BeginPlay: %s in %s has no attack montages."), *GetName(), *GetOwner()->GetName());
		UActorComponent::SetActive(false);
		return;
	}

	WeaponMelee = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(WeaponMelee))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UMeleeMontagesManager::BeginPlay: %s in %s is not in a WeaponMelee."), *GetName(), *GetOwner()->GetName());
		UActorComponent::SetActive(false);
		return;
	}

	WeaponMelee->OnWeaponAttackInterrupt.AddDynamic(this, &UMeleeMontagesManager::OnWeaponAttackInterrupted);
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

void UMeleeMontagesManager::OnWeaponAttackInterrupted()
{
	if (!IsValid(CurrentAttackMontage->AttackInterruptMontage.GetCharacterMontage()))
		return;

	FWeaponMeleeAttackMontageData* WeaponMontage = CurrentAttackMontage;
	bIsInterrupting = true;
	EndAttackSequence();
	WeaponMontage->AttackInterruptMontage.OnMontageFinished.AddUniqueDynamic(this, &UMeleeMontagesManager::OnAttackInterruptFinished);
	StartWeaponMontage(
		WeaponMontage->AttackInterruptMontage,
		1.0f,
		1.0f
	);
}

void UMeleeMontagesManager::OnAttackInterruptFinished(bool bInterrupted)
{
	bIsInterrupting = false;
}

void UMeleeMontagesManager::StartComboAttack()
{
	if (AttackMontages.IsEmpty())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeMontagesManager::StartComboAttack: %s in %s has no attack montages."), *GetName(), *GetOwner()->GetName());
		return;
	}

	ResetComboAttack();
	FWeaponMeleeAttackMontageData& AttackMontage = AttackMontages[0];

	FAlphaBlendArgs WeaponBlendArgs;
	if (IsValid(DefensiveMontage.GetWeaponMontage()))
		WeaponBlendArgs = DefensiveMontage.GetWeaponMontage()->BlendOut;

	CurrentAttackMontage = &AttackMontage;
	StartWeaponMontageWithBlends(
		AttackMontage.AttackMontage,
		1.0f, // Use the default play rate, since it can be set in the montage itself
		1.0f,
		WeaponBlendArgs,
		StartBlendIn
	);
}

void UMeleeMontagesManager::EndComboAttack() const
{
	if (CurrentAttackMontage == nullptr || !IsValid(CurrentAttackMontage->AttackMontage.GetCharacterMontage()))
		return;

	FAlphaBlendArgs WeaponBlendArgs;
	if (IsValid(DefensiveMontage.GetWeaponMontage()))
		WeaponBlendArgs = DefensiveMontage.GetWeaponMontage()->BlendOut;

	StopWeaponMontageWithBlends(
		CurrentAttackMontage->AttackMontage,
		WeaponBlendArgs,
		StopBlendOut
	);
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
	CurrentAttackIndex %= AttackMontages.Num();
	PlayAttackMontageAt(CurrentAttackIndex);
}

void UMeleeMontagesManager::PlayAttackMontageAt(const int32 AttackIndex)
{
	if (!AttackMontages.IsValidIndex(AttackIndex))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UMeleeMontagesManager::PlayAttackMontageAt: Invalid index %d for attack montage in %s."), AttackIndex, *GetName());
		return;
	}

	FWeaponMeleeAttackMontageData& AttackMontage = AttackMontages[AttackIndex];
	PlayAttackMontage(AttackMontage);
}

void UMeleeMontagesManager::PlayAttackMontage(FWeaponMeleeAttackMontageData& AttackMontage)
{
	CurrentAttackMontage = &AttackMontage;

	StartWeaponMontage(
		AttackMontage.AttackMontage,
		1.0f,
		1.0f
	);
}

void UMeleeMontagesManager::EndAttackSequence()
{
	EndComboAttack();
	ResetComboAttack();
	CurrentAttackMontage = nullptr;
}

void UMeleeMontagesManager::PlayDefensiveMontage()
{
	if (bIsDefenseInCooldown || bIsAttacking || bWantsToAttack)
		return;

	if (IsMontageDefending())
		return;

	if (!IsValid(DefensiveMontage.GetCharacterMontage()))
		return;

	bIsDefending = true;
	StartWeaponMontage(
		DefensiveMontage,
		1.0f,
		1.0f
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
	if (DefenseCooldown <= 0.0f)
		return;

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

void UMeleeMontagesManager::ResetDefenseCooldown()
{
	bIsDefenseInCooldown = false;
}
