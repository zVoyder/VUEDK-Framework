// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Montages/Data/WeaponAnimMetaData.h"
#include "Weapons/WeaponBase.h"
#include "WeaponNotifyStateBase.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UWeaponAnimMetaData* WeaponAnimMetaData = nullptr;
	bool bIsInitialized = false;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon);
	
	virtual void OnWeaponNotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon);

	virtual void OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon);
};
