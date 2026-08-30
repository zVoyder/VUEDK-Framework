// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies/WeaponEquipHideStateNotify.h"

void UWeaponEquipHideStateNotify::OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon)
{
	Super::OnWeaponNotifyBegin(MeshComp, Animation, TotalDuration, Weapon);
	Weapon->GetWeaponMesh()->SetVisibility(false);
}

void UWeaponEquipHideStateNotify::OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
	Super::OnWeaponNotifyEnd(MeshComp, Animation, EventReference, Weapon);
	Weapon->GetWeaponMesh()->SetVisibility(true);
}
