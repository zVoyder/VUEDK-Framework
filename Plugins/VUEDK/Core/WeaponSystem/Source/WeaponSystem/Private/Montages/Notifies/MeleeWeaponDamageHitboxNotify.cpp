// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies/MeleeWeaponDamageHitboxNotify.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponMelee.h"

void UMeleeWeaponDamageHitboxNotify::OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon)
{
	AWeaponMelee* MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UMeleeWeaponNextAttackIndex::OnWeaponNotifyBegin_Implementation: Invalid MeleeWeapon."));
		return;
	}
	
	MeleeWeapon->EnableDamageHitbox();
}

void UMeleeWeaponDamageHitboxNotify::OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
	AWeaponMelee* MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UMeleeWeaponNextAttackIndex::OnWeaponNotifyEnd_Implementation: Invalid MeleeWeapon."));
		return;
	}
	
	MeleeWeapon->DisableDamageHitbox();
}
