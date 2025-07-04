// Copyright zVoyder, Inc. All Rights Reserved.

#include "Montages/FirearmMontagesManager.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponFirearm.h"

UFirearmMontagesManager::UFirearmMontagesManager(): WeaponFirearm(nullptr)
{
}

void UFirearmMontagesManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check())
		return;

	WeaponFirearm->OnWeaponAttackSuccessEvent.RemoveDynamic(this, &UFirearmMontagesManager::OnWeaponAttackSuccess);
	WeaponFirearm->OnWeaponAttackFailEvent.RemoveDynamic(this, &UFirearmMontagesManager::OnWeaponAttackFail);
}

bool UFirearmMontagesManager::IsAnimShooting() const
{
	if (!Check())
		return false;
	
	return WeaponFirearm->IsPlayingWeaponMontage(ShootMontage);
}

bool UFirearmMontagesManager::IsAnimFailShooting() const
{
	if (!Check())
		return false;
	
	return WeaponFirearm->IsPlayingWeaponMontage(FailShootMontage);
}

void UFirearmMontagesManager::BeginPlay()
{
	Super::BeginPlay();

	WeaponFirearm = Cast<AWeaponFirearm>(Weapon);

	if (!IsValid(WeaponFirearm))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UFirearmMontagesManager::BeginPlay: %s in %s is not in a WeaponFirearm."), *GetName(), *GetOwner()->GetName());
		UActorComponent::SetActive(false);
	}
}

bool UFirearmMontagesManager::Check() const
{
	return Super::Check() && IsValid(WeaponFirearm);
}

void UFirearmMontagesManager::OnWeaponAttackSuccess()
{
	if (IsAnimShooting()) // Safe check
		return;

	float WeaponPlayRate;
	float CharacterPlayRate;
	GetShootPlayRates(ShootMontage, WeaponPlayRate, CharacterPlayRate);

	StartWeaponMontage(
		ShootMontage,
		WeaponPlayRate,
		CharacterPlayRate
	);
}

void UFirearmMontagesManager::OnWeaponAttackFail()
{
	if (IsAnimFailShooting()) // Safe check
		return;

	float WeaponPlayRate;
	float CharacterPlayRate;
	GetShootPlayRates(FailShootMontage, WeaponPlayRate, CharacterPlayRate);

	StartWeaponMontage(
		FailShootMontage,
		WeaponPlayRate,
		CharacterPlayRate
	);
}

void UFirearmMontagesManager::GetShootPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const
{
	WeaponPlayRate = 0.f;
	CharacterPlayRate = 0.f;
	const float SecondsPerShot = 60.f / WeaponFirearm->GetWeaponFireRate();
	
	if (IsValid(WeaponMontageData.GetWeaponMontage()))
		WeaponPlayRate = WeaponMontageData.GetWeaponMontage()->GetPlayLength() / SecondsPerShot;

	if (IsValid(WeaponMontageData.GetCharacterMontage()))
		CharacterPlayRate = WeaponMontageData.GetCharacterMontage()->GetPlayLength() / SecondsPerShot;
}
