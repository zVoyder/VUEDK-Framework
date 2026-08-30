// Copyright VUEDK, Inc. All Rights Reserved.

#include "WeaponCrosshair/WeaponCrosshairComponent.h"
#include "WeaponSystem.h"

UWeaponCrosshairComponent::UWeaponCrosshairComponent(): Weapon(nullptr)
{
}

void UWeaponCrosshairComponent::BeginPlay()
{
	Super::BeginPlay();

	Weapon = Cast<AWeaponBase>(GetOwner());

	if (!IsValid(Weapon))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UWeaponCrosshairBase::BeginPlay: Owner is not a weapon."));
		return;
	}
	
	SetPayload(Weapon);
	Weapon->OnWeaponEquippedEvent.AddDynamic(this, &UWeaponCrosshairComponent::OnWeaponEquipped);
	Weapon->OnWeaponUnequippedEvent.AddDynamic(this, &UWeaponCrosshairComponent::OnWeaponUnequipped);
}

void UWeaponCrosshairComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!Check())
		return;

	Weapon->OnWeaponEquippedEvent.RemoveDynamic(this, &UWeaponCrosshairComponent::OnWeaponEquipped);
	Weapon->OnWeaponUnequippedEvent.RemoveDynamic(this, &UWeaponCrosshairComponent::OnWeaponUnequipped);
}

void UWeaponCrosshairComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetOwner()->IsA(AWeaponBase::StaticClass()))
		UE_LOG(LogWeaponSystem, Error, TEXT("UWeaponCrosshairBase::OnRegister: Owner is not a weapon."));
}

void UWeaponCrosshairComponent::OnWeaponEquipped_Implementation()
{
	SetCrosshair();
}

void UWeaponCrosshairComponent::OnWeaponUnequipped_Implementation()
{
	SetGeneralCrosshair();
}

bool UWeaponCrosshairComponent::Check() const
{
	return IsValid(Weapon);
}
