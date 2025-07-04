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
	/**
	 * Initializes the notify state with the given weapon.
	 * @param Weapon The weapon to initialize the notify state with.
	 */
	void Init(AWeaponBase* Weapon);

	/**
	 * Checks if the notify state is properly initialized.
	 * @return True if the notify state is initialized, false otherwise.
	 */
	bool Check() const;
};
