// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapons/WeaponFirearm.h"
#include "ReloadInsertAmmoNotify.generated.h"

UCLASS()
class WEAPONSYSTEM_API UReloadInsertAmmoNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Index of the reload behaviour to reload"))
	int32 ReloadBehaviourIndex = 0;
	
private:
	UPROPERTY()
	AWeaponFirearm* Weapon;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
