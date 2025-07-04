// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies/MeleeWeaponAttackStateNotify.h"
#include "Weapons/WeaponMelee.h"

void UMeleeWeaponAttackStateNotify::OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon)
{
	const AWeaponMelee* MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
		return;

	MeleeWeapon->MeleeMontagesManager->OnAttackBeginNotify();
}

void UMeleeWeaponAttackStateNotify::OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
	const AWeaponMelee* MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
		return;

	MeleeWeapon->MeleeMontagesManager->OnAttackFinishedNotify();
}
