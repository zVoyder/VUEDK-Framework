// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies/MeleeWeaponBufferAttackStateNotify.h"
#include "Weapons/WeaponMelee.h"

void UMeleeWeaponBufferAttackStateNotify::OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon)
{
	Super::OnWeaponNotifyBegin(MeshComp, Animation, TotalDuration, Weapon);
	Init(Weapon);
}

void UMeleeWeaponBufferAttackStateNotify::OnWeaponNotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
	if (!Check())
		return;

	MeleeWeapon->MeleeMontagesManager->CheckBufferAttack();
}

void UMeleeWeaponBufferAttackStateNotify::Init(AWeaponBase* Weapon)
{
	MeleeWeapon = Cast<AWeaponMelee>(Weapon);

	if (!IsValid(MeleeWeapon))
		return;
	
	bIsInitialized = true;
}

bool UMeleeWeaponBufferAttackStateNotify::Check() const
{
	return bIsInitialized;
}
