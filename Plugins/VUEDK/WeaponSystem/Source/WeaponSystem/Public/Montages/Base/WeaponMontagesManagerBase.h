// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Components/ActorComponent.h"
#include "WeaponMontagesManagerBase.generated.h"

UCLASS(Abstract, NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UWeaponMontagesManagerBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	FWeaponMontageData EquipMontageData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	FWeaponMontageData UnequipMontageData;

protected:
	UPROPERTY()
	AWeaponBase* Weapon;

private:
	bool bIsPlayingEquipMontage;

public:
	UWeaponMontagesManagerBase();

	/**
	 * Resumes the specified weapon montage.
	 * @param WeaponMontageData - The montage data to resume.
	 */
	void ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	/**
	 * Pauses the specified weapon montage.
	 * @param WeaponMontageData - The montage data to pause.
	 */
	void PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	/**
	 * Starts the specified weapon montage with given play rates.
	 * @param WeaponMontageData - The montage data to start.
	 * @param WeaponPlayRate - The play rate for the weapon montage.
	 * @param CharacterPlayRate - The play rate for the character animation.
	 */
	void StartWeaponMontage(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate) const;
	
	/**
	 * Starts the specified weapon montage with custom blends for weapon and character.
	 * @param WeaponMontageData - The montage data to start.
	 * @param WeaponPlayRate - The play rate for the weapon montage.
	 * @param CharacterPlayRate - The play rate for the character animation.
	 * @param WeaponBlendIn - The blend in arguments for the weapon.
	 * @param CharacterBlendIn - The blend in arguments for the character.
	 */
	void StartWeaponMontageWithBlends(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn) const;

	/**
	 * Stops the specified weapon montage.
	 * @param WeaponMontageData - The montage data to stop.
	 */
	void StopWeaponMontage(const FWeaponMontageData WeaponMontageData) const;
	
	/**
	 * Stops the specified weapon montage with custom blend out settings for weapon and character.
	 * @param WeaponMontageData - The montage data to stop.
	 * @param WeaponBlendOut - The blend out arguments for the weapon.
	 * @param CharacterBlendOut - The blend out arguments for the character.
	 */
	void StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut) const;
	
	/**
	 * Checks if the specified weapon montage is currently playing.
	 * @param WeaponMontageData - The montage data to check.
	 * @return true if the montage is playing, false otherwise.
	 */
	bool IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;

	/**
	 * Checks if the manager is currently busy (e.g., playing a montage).
	 * @return true if busy, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool IsBusy() const;
	
	/**
	 * Checks if an equip or unequip montage is currently playing.
	 * @return true if an equip or unequip montage is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEquipOrUnequipMontagePlaying() const;
	
	/**
	 * Called when the component ends play. Used for cleanup logic.
	 * @param EndPlayReason - The reason the component is ending play.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	/**
	 * Called when the component begins play. Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Checks if the manager is in a valid state for operations.
	 * @return true if valid, false otherwise.
	 */
	virtual bool Check() const;

	/**
	 * Called when a weapon attack succeeds.
	 */
	UFUNCTION()
	virtual void OnWeaponAttackSuccess();

	/**
	 * Called when a weapon attack fails.
	 */
	UFUNCTION()
	virtual void OnWeaponAttackFail();

	/**
	 * Called when a weapon attack ends.
	 */
	UFUNCTION()
	virtual void OnWeaponEndAttack();

private:
	/**
	 * Starts the equip montage for the weapon.
	 */
	void StartEquipMontage();
	
	/**
	 * Starts the unequip montage for the weapon.
	 */
	void StartUnequipMontage();
	
	/**
	 * Called when the weapon is equipped.
	 */
	UFUNCTION()
	void OnWeaponEquipped();

	/**
	 * Called when the weapon is unequipped.
	 */
	UFUNCTION()
	void OnWeaponUnequipped();

	/**
	 * Called when the weapon is ready to use (after equip/unequip).
	 * @param bInterrupted - True if the process was interrupted, false otherwise.
	 */
	UFUNCTION()
	void OnWeaponReadyToUse(bool bInterrupted);
};
