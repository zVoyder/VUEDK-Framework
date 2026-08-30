// Copyright VUEDK, Inc. All Rights Reserved.

#include "WeaponCrosshair/Widgets/WeaponCrosshairWidget.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponFirearm.h"

void UWeaponCrosshairWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!IsValid(Weapon))
		return;

	Weapon->OnWeaponAttacked.RemoveDynamic(this, &UWeaponCrosshairWidget::OnWeaponAttackSuccess);
	Weapon->OnWeaponAttackFailed.RemoveDynamic(this, &UWeaponCrosshairWidget::OnWeaponAttackFailed);
}

void UWeaponCrosshairWidget::OnInit_Implementation()
{
	Weapon = Cast<AWeaponBase>(GetPayload());

	if (!IsValid(Weapon))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UWeaponCrosshairWidget::OnInit_Implementation: Weapon is null"));
		return;
	}

	Weapon->OnWeaponAttacked.AddDynamic(this, &UWeaponCrosshairWidget::OnWeaponAttackSuccess);
	Weapon->OnWeaponAttackFailed.AddDynamic(this, &UWeaponCrosshairWidget::OnWeaponAttackFailed);
}

void UWeaponCrosshairWidget::OnWeaponAttackSuccess_Implementation()
{
}

void UWeaponCrosshairWidget::OnWeaponAttackFailed_Implementation()
{
}
