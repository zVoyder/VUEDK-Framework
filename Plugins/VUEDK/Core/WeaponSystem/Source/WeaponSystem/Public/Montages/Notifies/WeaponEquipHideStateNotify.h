// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponNotifyStateBase.h"
#include "WeaponEquipHideStateNotify.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponEquipHideStateNotify : public UWeaponNotifyStateBase
{
	GENERATED_BODY()

protected:
	virtual void OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon) override;

	virtual void OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon) override;
};
