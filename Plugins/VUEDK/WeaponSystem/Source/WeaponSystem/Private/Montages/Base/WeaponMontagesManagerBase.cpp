// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Base/WeaponMontagesManagerBase.h"
#include "WeaponSystem.h"

UWeaponMontagesManagerBase::UWeaponMontagesManagerBase(): Weapon(nullptr),
                                                          bIsPlayingEquipMontage(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponMontagesManagerBase::ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (!Check())
		return;

	Weapon->ResumeWeaponMontage(WeaponMontageData);
}

void UWeaponMontagesManagerBase::PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (!Check())
		return;

	Weapon->PauseWeaponMontage(WeaponMontageData);
}

void UWeaponMontagesManagerBase::StartWeaponMontage(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate) const
{
	if (!Check())
		return;

	Weapon->StartWeaponMontage(
		WeaponMontageData,
		WeaponPlayRate,
		CharacterPlayRate
	);
}

void UWeaponMontagesManagerBase::StartWeaponMontageWithBlends(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn) const
{
	if (!Check())
		return;

	Weapon->StartWeaponMontageWithBlends(
		WeaponMontageData,
		WeaponPlayRate,
		CharacterPlayRate,
		WeaponBlendIn,
		CharacterBlendIn
	);
}

void UWeaponMontagesManagerBase::StopWeaponMontage(const FWeaponMontageData WeaponMontageData) const
{
	if (!Check())
		return;

	Weapon->StopWeaponMontage(WeaponMontageData);
}

void UWeaponMontagesManagerBase::StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut) const
{
	if (!Check())
		return;

	Weapon->StopWeaponMontageWithBlends(
		WeaponMontageData,
		WeaponBlendOut,
		CharacterBlendOut
	);
}

bool UWeaponMontagesManagerBase::IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (!Check())
		return false;

	return Weapon->IsPlayingWeaponMontage(WeaponMontageData);
}

bool UWeaponMontagesManagerBase::IsBusy_Implementation() const
{
	return IsEquipOrUnequipMontagePlaying();
}

bool UWeaponMontagesManagerBase::IsEquipOrUnequipMontagePlaying() const
{
	return bIsPlayingEquipMontage;
}

void UWeaponMontagesManagerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(Weapon))
	{
		Weapon->OnWeaponAttackSuccessEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackSuccess);
		Weapon->OnWeaponAttackFailEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackFail);
		Weapon->OnEndWeaponAttackEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEndAttack);
		Weapon->OnWeaponEquippedEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEquipped);
		Weapon->OnWeaponUnequippedEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponUnequipped);
	}
}

void UWeaponMontagesManagerBase::BeginPlay()
{
	Super::BeginPlay();

	Weapon = Cast<AWeaponBase>(GetOwner());

	if (!IsValid(Weapon)) // Do not use Check(), since it is virtual
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UWeaponMontagesManagerBase::OnRegister: %s in %s is not in a Weapon."), *GetName(), *GetOwner()->GetName());
		UActorComponent::SetActive(false);
	}

	Weapon->OnWeaponAttackSuccessEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackSuccess);
	Weapon->OnWeaponAttackFailEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackFail);
	Weapon->OnEndWeaponAttackEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEndAttack);
	Weapon->OnWeaponEquippedEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEquipped);
	Weapon->OnWeaponUnequippedEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponUnequipped);
}

bool UWeaponMontagesManagerBase::Check() const
{
	return IsValid(Weapon);
}

void UWeaponMontagesManagerBase::OnWeaponAttackSuccess()
{
}

void UWeaponMontagesManagerBase::OnWeaponAttackFail()
{
}

void UWeaponMontagesManagerBase::OnWeaponEndAttack()
{
}

void UWeaponMontagesManagerBase::StartEquipMontage()
{
	if (EquipMontageData.GetCharacterMontage() == nullptr && EquipMontageData.GetWeaponMontage() == nullptr)
		return;

	bIsPlayingEquipMontage = true;
	EquipMontageData.OnMontageFinished.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponReadyToUse);
	StartWeaponMontage(EquipMontageData, 1.0f, 1.0f);
}

void UWeaponMontagesManagerBase::StartUnequipMontage()
{
	if (UnequipMontageData.GetCharacterMontage() == nullptr && UnequipMontageData.GetWeaponMontage() == nullptr)
		return;

	bIsPlayingEquipMontage = true;
	UnequipMontageData.OnMontageFinished.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponReadyToUse);
	StartWeaponMontage(UnequipMontageData, 1.0f, 1.0f);
}

void UWeaponMontagesManagerBase::OnWeaponEquipped()
{
	StartEquipMontage();
}

void UWeaponMontagesManagerBase::OnWeaponUnequipped()
{
	StartUnequipMontage();
}

void UWeaponMontagesManagerBase::OnWeaponReadyToUse(bool bInterrupted)
{
	bIsPlayingEquipMontage = false;
}
