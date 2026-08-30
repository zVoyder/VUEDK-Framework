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

public:
	UWeaponNotifyStateBase();
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	/**
	 * Initializes the notify state with the given weapon animation metadata.
	 * @param MeshComp The skeletal mesh component to initialize with.
	 * @param Animation The animation sequence base to initialize with.
	 * @param TotalDuration The total duration of the animation.
	 * @param Weapon The weapon associated with this notify state.
	 */
	virtual void OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon);

	/**
	 * Called every tick during the notify state.
	 * @param MeshComp The skeletal mesh component associated with the notify.
	 * @param Animation The animation sequence base being played.
	 * @param FrameDeltaTime The time delta for the current frame.
	 * @param EventReference Reference to the animation notify event.
	 * @param Weapon The weapon associated with this notify state.
	 */
	virtual void OnWeaponNotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon);

	/**
	 * Called when the notify state ends.
	 * @param MeshComp The skeletal mesh component associated with the notify.
	 * @param Animation The animation sequence base being played.
	 * @param EventReference Reference to the animation notify event.
	 * @param Weapon The weapon associated with this notify state.
	 */
	virtual void OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon);
};
