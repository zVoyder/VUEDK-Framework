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

private:
	UPROPERTY()
	AWeaponFirearm* Weapon;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION(BlueprintPure)
	AWeaponFirearm* GetWeaponFirearm() const;

	UFUNCTION(BlueprintPure)
	bool Check() const;
	
private:
	void Init(AActor* InOwner);
};
