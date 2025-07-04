// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponNotifyStateBase.h"
#include "Weapons/WeaponMelee.h"
#include "MeleeWeaponBufferAttackStateNotify.generated.h"

UCLASS()
class WEAPONSYSTEM_API UMeleeWeaponBufferAttackStateNotify : public UWeaponNotifyStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AWeaponMelee* MeleeWeapon;
	bool bIsInitialized = false;

protected:
	virtual void OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon) override;

	virtual void OnWeaponNotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon) override;
	
private:
	void Init(AWeaponBase* Weapon);

	bool Check() const;
};
