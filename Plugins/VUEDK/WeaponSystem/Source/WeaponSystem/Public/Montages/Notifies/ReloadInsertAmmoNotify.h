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

public:
	/**
	 * Initializes the notify with the owner actor.
	 * @param InOwner - The actor that owns this notify.
	 */
	void Init(AActor* InOwner);

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	/**
	 * Gets the weapon firearm associated with this notify.
	 * @return Pointer to the AWeaponFirearm instance, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure)
	AWeaponFirearm* GetWeaponFirearm() const;

	/**
	 * Checks if the notify is valid and ready for operations.
	 * @return true if valid, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool Check() const;
};
