// Copyright zVoyder, Inc. All Rights Reserved.

#include "Montages/FirearmMontagesManager.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponFirearm.h"
#include "Weapons/Data/WeaponShootData.h"

UFirearmMontagesManager::UFirearmMontagesManager() : WeaponFirearm(nullptr)
{
}

bool UFirearmMontagesManager::TryPlayReloadMontage(const int32 MontageIndex)
{
	if (!Check())
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UFirearmMontagesManager::PlayReloadMontage: Check failed."));
		return false;
	}

	if (!ReloadMontages.IsValidIndex(MontageIndex))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UFirearmMontagesManager::PlayReloadMontage: Invalid ReloadMontageIndex %d."), MontageIndex);
		return false;
	}

	float WeaponPlayRate;
	float CharacterPlayRate;
	GetReloadPlayRates(ReloadMontages[MontageIndex], WeaponPlayRate, CharacterPlayRate);
	StartWeaponMontage(
		ReloadMontages[MontageIndex],
		WeaponPlayRate,
		CharacterPlayRate
	);

	return true;
}

bool UFirearmMontagesManager::TryPlayShootMontage(const int32 MontageIndex)
{
	if (ShootSuccessMontages.IsEmpty())
		return false;

	if (!ShootSuccessMontages.IsValidIndex(MontageIndex))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UFirearmMontagesManager::PlayShootMontage: Invalid ShootMontageIndex %d."), MontageIndex);
		return false;
	}

	float WeaponPlayRate;
	float CharacterPlayRate;
	GetShootPlayRates(ShootSuccessMontages[MontageIndex], WeaponPlayRate, CharacterPlayRate);
	StartWeaponMontage(
		ShootSuccessMontages[MontageIndex],
		WeaponPlayRate,
		CharacterPlayRate
	);
	
	return true;
}

bool UFirearmMontagesManager::TryPlayShootFailMontage(const int32 MontageIndex)
{
	if (ShootFailMontages.IsEmpty())
		return false;

	if (!ShootFailMontages.IsValidIndex(MontageIndex))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UFirearmMontagesManager::PlayShootFailMontage: Invalid ShootFailMontageIndex %d."), MontageIndex);
		return false;
	}

	float WeaponPlayRate;
	float CharacterPlayRate;
	GetShootPlayRates(ShootFailMontages[MontageIndex], WeaponPlayRate, CharacterPlayRate);
	StartWeaponMontage(
		ShootFailMontages[MontageIndex],
		WeaponPlayRate,
		CharacterPlayRate
	);
	
	return true;
}

void UFirearmMontagesManager::StopShootMontage(const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut)
{
	for (const FWeaponMontageData& ShootMontage : ShootSuccessMontages)
		StopWeaponMontageWithBlends(ShootMontage, WeaponBlendOut, CharacterBlendOut);
}

void UFirearmMontagesManager::StopShootFailMontage(const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut)
{
	for (const FWeaponMontageData& ShootFailMontage : ShootFailMontages)
		StopWeaponMontageWithBlends(ShootFailMontage, WeaponBlendOut, CharacterBlendOut);
}

void UFirearmMontagesManager::StopReloadMontage(const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut)
{
	for (const FWeaponMontageData& ReloadMontage : ReloadMontages)
		StopWeaponMontageWithBlends(ReloadMontage, WeaponBlendOut, CharacterBlendOut);
}

bool UFirearmMontagesManager::IsAnimShooting() const
{
	for (const FWeaponMontageData& ShootMontage : ShootSuccessMontages)
	{
		if (IsPlayingWeaponMontage(ShootMontage))
			return true;
	}

	return false;
}

bool UFirearmMontagesManager::IsAnimShootFailing() const
{
	for (const FWeaponMontageData& ShootFailMontage : ShootFailMontages)
	{
		if (IsPlayingWeaponMontage(ShootFailMontage))
			return true;
	}

	return false;
}

bool UFirearmMontagesManager::IsAnimReloading() const
{
	for (const FWeaponMontageData& ReloadMontage : ReloadMontages)
	{
		if (IsPlayingWeaponMontage(ReloadMontage))
			return true;
	}

	return false;
}

void UFirearmMontagesManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check())
		return;

	WeaponFirearm->OnWeaponAttacked.RemoveDynamic(this, &UFirearmMontagesManager::OnWeaponAttackSuccess);
	WeaponFirearm->OnWeaponAttackFailed.RemoveDynamic(this, &UFirearmMontagesManager::OnWeaponAttackFail);
}

void UFirearmMontagesManager::SetupWeapons(AWeaponBase* InWeapon)
{
	Super::SetupWeapons(InWeapon);
	WeaponFirearm = Cast<AWeaponFirearm>(InWeapon);
}

void UFirearmMontagesManager::BindEvents()
{
	Super::BindEvents();
	WeaponFirearm->Shooter->OnBehaviourShootSuccess.AddDynamic(this, &UFirearmMontagesManager::OnBehaviourShootSuccess);
	WeaponFirearm->Shooter->OnBehaviourShootFail.AddDynamic(this, &UFirearmMontagesManager::OnBehaviourShootFail);
}

void UFirearmMontagesManager::UnbindEvents()
{
	Super::UnbindEvents();
	WeaponFirearm->Shooter->OnBehaviourShootSuccess.RemoveDynamic(this, &UFirearmMontagesManager::OnBehaviourShootSuccess);
	WeaponFirearm->Shooter->OnBehaviourShootFail.RemoveDynamic(this, &UFirearmMontagesManager::OnBehaviourShootFail);
}

bool UFirearmMontagesManager::Check() const
{
	return Super::Check() && IsValid(WeaponFirearm) && IsValid(WeaponFirearm->Shooter);
}

void UFirearmMontagesManager::OnBehaviourShootSuccess(UShooterBehaviourBase* Behaviour, UShootBarrel* ShootBarrel, int32 ShotIndex)
{
	if (IsAnimShooting())
		return;

	int32 MontageIndex;
	if (!Behaviour->TryGetShootSuccessMontageIndex(MontageIndex, ShotIndex))
		return;
	
	TryPlayShootMontage(MontageIndex);
}

void UFirearmMontagesManager::OnBehaviourShootFail(UShooterBehaviourBase* Behaviour, EShootFailReason FailReason)
{
	if (IsAnimShootFailing())
		return;

	int32 MontageIndex;
	if (!Behaviour->TryGetShootFailMontageIndex(MontageIndex, FailReason))
		return;
	
	TryPlayShootFailMontage(MontageIndex);
}

void UFirearmMontagesManager::OnWeaponMontageBegin(const FWeaponMontageData& WeaponMontageData)
{
	Super::OnWeaponMontageBegin(WeaponMontageData);
	HandleShootSuccessMontageBegin(WeaponMontageData);
	HandleShootFailMontageBegin(WeaponMontageData);
	HandleReloadMontageBegin(WeaponMontageData);
}

void UFirearmMontagesManager::OnWeaponMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted)
{
	Super::OnWeaponMontageFinished(WeaponMontageData, bInterrupted);
	HandleShootSuccessMontageFinished(WeaponMontageData, bInterrupted);
	HandleShootFailMontageFinished(WeaponMontageData, bInterrupted);
	HandleReloadMontageFinished(WeaponMontageData, bInterrupted);
}

void UFirearmMontagesManager::GetShootPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const
{
	const float SecondsPerShot = 60.f / WeaponFirearm->GetWeaponFireRate();
	const UAnimMontage* WeaponMontage = WeaponMontageData.GetWeaponMontage();
	const UAnimMontage* CharacterMontage = WeaponMontageData.GetCharacterMontage();

	WeaponPlayRate = IsValid(WeaponMontage) ? WeaponMontage->GetPlayLength() / SecondsPerShot : 1.f;
	CharacterPlayRate = IsValid(CharacterMontage) ? CharacterMontage->GetPlayLength() / SecondsPerShot : 1.f;
}

void UFirearmMontagesManager::GetReloadPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const
{
	const float ReloadTime = WeaponFirearm->GetWeaponReloadTime();
	const UAnimMontage* WeaponMontage = WeaponMontageData.GetWeaponMontage();
	const UAnimMontage* CharacterMontage = WeaponMontageData.GetCharacterMontage();

	WeaponPlayRate = IsValid(WeaponMontage) ? WeaponMontage->GetPlayLength() / ReloadTime : 1.f;
	CharacterPlayRate = IsValid(CharacterMontage) ? CharacterMontage->GetPlayLength() / ReloadTime : 1.f;
}

void UFirearmMontagesManager::HandleShootSuccessMontageBegin(const FWeaponMontageData& WeaponMontageData) const
{
	if (!ShootSuccessMontages.Contains(WeaponMontageData))
		return;

	const int32 Index = ShootSuccessMontages.IndexOfByKey(WeaponMontageData);
	OnShootMontagePlayed.Broadcast(WeaponMontageData, Index);
}

void UFirearmMontagesManager::HandleShootFailMontageBegin(const FWeaponMontageData& WeaponMontageData) const
{
	if (!ShootFailMontages.Contains(WeaponMontageData))
		return;

	const int32 Index = ShootFailMontages.IndexOfByKey(WeaponMontageData);
	OnShootFailMontagePlayed.Broadcast(WeaponMontageData, Index);
}

void UFirearmMontagesManager::HandleReloadMontageBegin(const FWeaponMontageData& WeaponMontageData) const
{
	if (!ReloadMontages.Contains(WeaponMontageData))
		return;

	const int32 Index = ReloadMontages.IndexOfByKey(WeaponMontageData);
	OnReloadMontagePlayed.Broadcast(WeaponMontageData, Index);
}

void UFirearmMontagesManager::HandleShootSuccessMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) const
{
	if (!ShootSuccessMontages.Contains(WeaponMontageData))
		return;

	const int32 Index = ShootSuccessMontages.IndexOfByKey(WeaponMontageData);
	OnShootMontageEnded.Broadcast(WeaponMontageData, Index, bInterrupted);
}

void UFirearmMontagesManager::HandleShootFailMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) const
{
	if (!ShootFailMontages.Contains(WeaponMontageData))
		return;

	const int32 Index = ShootFailMontages.IndexOfByKey(WeaponMontageData);
	OnShootFailMontageEnded.Broadcast(WeaponMontageData, Index, bInterrupted);
}

void UFirearmMontagesManager::HandleReloadMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) const
{
	if (!ReloadMontages.Contains(WeaponMontageData))
		return;

	const int32 Index = ReloadMontages.IndexOfByKey(WeaponMontageData);
	OnReloadMontageEnded.Broadcast(WeaponMontageData, Index, bInterrupted);
}
