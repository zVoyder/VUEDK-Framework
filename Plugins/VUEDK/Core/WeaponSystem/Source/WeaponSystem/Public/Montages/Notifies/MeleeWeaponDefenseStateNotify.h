// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponNotifyStateBase.h"
#include "MeleeWeaponDefenseStateNotify.generated.h"

UCLASS()
class WEAPONSYSTEM_API UMeleeWeaponDefenseStateNotify : public UWeaponNotifyStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Notify")
	bool bDisableBlockOnExit = false;
	
protected:
	virtual void OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon) override;

	virtual void OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon) override;
};
