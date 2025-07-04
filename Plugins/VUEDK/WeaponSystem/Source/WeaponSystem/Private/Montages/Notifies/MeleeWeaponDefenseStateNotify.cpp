// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies/MeleeWeaponDefenseStateNotify.h"
#include "Weapons/WeaponMelee.h"

void UMeleeWeaponDefenseStateNotify::OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon)
{
	Super::OnWeaponNotifyBegin(MeshComp, Animation, TotalDuration, Weapon);

	AWeaponMelee* MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
		return;

	MeleeWeapon->SetBlockActive(true);
}

void UMeleeWeaponDefenseStateNotify::OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
	Super::OnWeaponNotifyEnd(MeshComp, Animation, EventReference, Weapon);

	if (bDisableBlockOnExit)
		return;
	
	AWeaponMelee* MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
		return;

	MeleeWeapon->SetBlockActive(false);
}
