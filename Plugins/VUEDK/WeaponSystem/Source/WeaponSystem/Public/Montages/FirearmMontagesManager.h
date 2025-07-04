// Copyright zVoyder, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponMontagesManagerBase.h"
#include "FirearmMontagesManager.generated.h"

class AWeaponFirearm;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UFirearmMontagesManager : public UWeaponMontagesManagerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData ReloadMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData ShootMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData FailShootMontage;

protected:
	UPROPERTY()
	AWeaponFirearm* WeaponFirearm;

public:
	UFirearmMontagesManager();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Checks if the shooting animation is currently playing.
	 * @return true if the shooting animation is playing, false otherwise.
	 */
	UFUNCTION()
	bool IsAnimShooting() const;

	/**
	 * Checks if the fail shooting animation is currently playing.
	 * @return true if the fail shooting animation is playing, false otherwise.
	 */
	UFUNCTION()
	bool IsAnimFailShooting() const;

protected:
	/**
	 * Called when the component begins play. Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Checks if the manager is in a valid state for operations.
	 * @return true if valid, false otherwise.
	 */
	virtual bool Check() const override;

private:
	/**
	 * Called when a weapon attack succeeds.
	 */
	virtual void OnWeaponAttackSuccess() override;

	/**
	 * Called when a weapon attack fails.
	 */
	virtual void OnWeaponAttackFail() override;

	/**
	 * Gets the play rates for the shoot montage for weapon and character.
	 * @param WeaponMontageData - The montage data to use.
	 * @param WeaponPlayRate - Output parameter for the weapon play rate.
	 * @param CharacterPlayRate - Output parameter for the character play rate.
	 */
	void GetShootPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const;
};
