// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Data/ReloadEventData.h"
#include "Data/WeaponFirearmData.h"
#include "Montages/FirearmMontagesManager.h"
#include "Shooter/Shooter.h"
#include "WeaponFirearm.generated.h"

class UShootBarrel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnReloadStarted,
	FReloadEventData, ReloadPayload
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnReloadInsertedAmmo,
	FReloadEventData, ReloadPayload
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnReloadEnded
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnReloadInterrupted,
	FReloadEventData, ReloadPayload
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnAimEnabled
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnAimDisabled
);

UCLASS()
class WEAPONSYSTEM_API AWeaponFirearm : public AWeaponBase
{
	GENERATED_BODY()

	friend class UReloadInsertAmmoNotify;

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadStarted OnReloadStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadEnded OnReloadEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadInsertedAmmo OnReloadInsertedAmmo;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadInterrupted OnReloadInterrupted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAimEnabled OnAimEnabled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAimDisabled OnAimDisabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UFirearmMontagesManager* FirearmMontagesManager;

protected:
	// -- Data --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FWeaponFirearmData WeaponFirearmData;

	// -- Configuration --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Configuration")
	EShootType DefaultShootType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Configuration")
	FName ShootBarrelSocketName;

	// -- Advanced --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Advanced")
	bool bCanDeployAttackIfReloading = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShootBarrel* ShootBarrel;

private:
	bool bIsAimingDownSight;
	bool bHasReloadInsertedAmmo;
	float AimSpread;
	float DefaultRecoilStrength;
	float AdsRecoilStrength;
	FReloadEventData ReloadPayload;

public:
	AWeaponFirearm();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	/**
	 * Initializes the weapon with the given owner and optional payload.
	 * @param InOwner - The pawn that owns this weapon.
	 * @param InPayload - Optional payload data for initialization.
	 */
	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr) override;

	/**
	 * Adds dynamic spread to the weapon, affecting aim stability.
	 * @param AddSpread - The amount of spread to add.
	 * @param ChangeRate - The rate at which the spread changes.
	 * @param RecoveryRate - The rate at which the spread recovers.
	 */
	UFUNCTION(BlueprintCallable)
	void AddWeaponDynamicSpread(const float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f) const;

	/**
	 * Resets the weapon's cooldown, allowing immediate use.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetCooldown() const;

	/**
	 * Resets the weapon's spread to its default value.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetSpread() const;

	/**
	 * Sets the damage dealt by the weapon.
	 * @param NewDamage - The new damage value.
	 */
	virtual void SetWeaponDamage(const float NewDamage) override;

	/**
	 * Sets the fire rate of the weapon.
	 * @param NewFireRate - The new fire rate value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponFireRate(const float NewFireRate) const;

	/**
	 * Sets the magazine size of the weapon.
	 * @param NewMagSize - The new magazine size.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponMagSize(const int32 NewMagSize) const;

	/**
	 * Sets the maximum range of the weapon.
	 * @param NewRange - The new maximum range.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponMaxRange(const float NewRange) const;

	/**
	 * Sets the recoil strength of the weapon.
	 * @param NewRecoilStrength - The new recoil strength.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponRecoilStrength(const float NewRecoilStrength) const;

	/**
	 * Sets the reload time of the weapon.
	 * @param NewReloadTime - The new reload time.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponReloadTime(const float NewReloadTime);

	/**
	 * Sets the current ammo count of the weapon.
	 * @param NewAmmo - The new ammo count.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo) const;

	/**
	 * Sets the ammo type of the weapon.
	 * @param NewAmmoType - Pointer to the new ammo type data.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponAmmoType(UAmmoTypeData* NewAmmoType);

	/**
	 * Sets the shoot type of the weapon.
	 * @param NewShootType - The new shoot type.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponShootType(const EShootType NewShootType) const;

	/**
	 * Instantly sets the spread of the weapon.
	 * @param InSpread - The new spread value.
	 * @param bOverrideDefault - Whether to override the default spread.
	 */
	UFUNCTION(BlueprintCallable)
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = true) const;

	/**
	 * Sets the weapon's spread, with an option to override the default value.
	 * @param InSpread - The new spread value.
	 * @param bOverrideDefault - Whether to override the default spread.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponSpread(const float InSpread, const bool bOverrideDefault = true) const;

	/**
	 * Gets the firearm data structure containing various weapon properties.
	 * @return The weapon data structure.
	 */
	UFUNCTION(BlueprintPure)
	FWeaponFirearmData GetWeaponFirearmData() const;

	/**
	 * Gets the current fire rate of the weapon.
	 * @return The fire rate as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponFireRate() const;

	/**
	 * Gets the current magazine size of the weapon.
	 * @return The magazine size as an integer.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponMagSize() const;

	/**
	 * Gets the maximum range of the weapon.
	 * @return The maximum range as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponMaxRange() const;

	/**
	 * Gets the recoil strength of the weapon.
	 * @return The recoil strength as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponRecoilStrength() const;

	/**
	 * Gets the current reload time of the weapon.
	 * @return The reload time as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponReloadTime() const;

	/**
	 * Gets the current ammo count of the weapon.
	 * @return The current ammo as an integer.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	/**
	 * Gets the amount of ammo needed to fully reload the weapon.
	 * @return The needed ammo as an integer.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetNeededAmmo() const;

	/**
	 * Gets the ammo type data used by this weapon.
	 * @return Pointer to the UAmmoTypeData instance.
	 */
	UFUNCTION(BlueprintPure)
	UAmmoTypeData* GetWeaponAmmoType() const;

	/**
	 * Gets the shoot type of the weapon (e.g., single, burst, auto).
	 * @return The EShootType enum value.
	 */
	UFUNCTION(BlueprintPure)
	EShootType GetWeaponShootType() const;

	/**
	 * Gets the maximum possible spread for this weapon.
	 * @return The max spread as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponMaxSpread() const;

	/**
	 * Gets the default spread value for this weapon.
	 * @return The default spread as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetDefaultSpread() const;

	/**
	 * Gets the current spread value for this weapon.
	 * @return The current spread as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponSpread() const;

	/**
	 * Resets the weapon's shoot type to its default value.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetToDefaultShootType() const;

	/**
	 * Reloads the weapon using a montage animation.
	 * @param AmmoData - The ammo type data to use for reloading.
	 * @param Ammo - The amount of ammo to reload.
	 */
	UFUNCTION(BlueprintCallable)
	void ReloadWithMontage(UAmmoTypeData* AmmoData, const int32 Ammo);

	/**
	 * Interrupts the reload process, optionally specifying blend out times.
	 * @param CharacterBlendOutTime - The blend out time for the character animation.
	 * @param WeaponBlendOutTime - The blend out time for the weapon animation.
	 */
	UFUNCTION(BlueprintCallable)
	void InterruptReload(float CharacterBlendOutTime = 0.0f, float WeaponBlendOutTime = 0.0f);

	/**
	 * Refills the weapon with the specified amount of ammo.
	 * @param Ammo - The amount of ammo to refill.
	 * @return The amount of ammo actually refilled.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 Refill(int32 Ammo) const;

	/**
	 * Refills all magazines of the weapon.
	 */
	UFUNCTION(BlueprintCallable)
	void RefillAllMagazine() const;

	/**
	 * Sets the aim state of the weapon.
	 * @param bIsEnabled - True to enable aiming, false to disable.
	 */
	UFUNCTION(BlueprintCallable)
	void SetAim(bool bIsEnabled);

	/**
	 * Checks if the weapon is currently aiming.
	 * @return true if aiming, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAiming() const;

	/**
	 * Checks if the weapon has just shot.
	 * @return true if the weapon has just shot, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasJustShot() const;

	/**
	 * Checks if the weapon is currently reloading.
	 * @return true if reloading, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsReloading() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Sets the weapon in the aim state, adjusting spread and recoil.
	 */
	virtual void EnableAim();

	/**
	 * Disables the aim state, resetting spread and recoil.
	 */
	virtual void DisableAim();

	/**
	 * Checks if the weapon can reload.
	 * @return True if the weapon can reload, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanReload() const;

	/**
	 * Called when the reload process ends successfully.
	 * @param Remain - The remaining ammo after reload.
	 * @param ReloadedAmmo - The amount of ammo that was reloaded.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadSuccess(float Remain, float ReloadedAmmo);

	/**
	 * Called when the reload fails.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadFail();

	/**
	 * Called when the weapon goes into Aim state.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEnableAim();

	/**
	 * Called when the weapon exits Aim state.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDisableAim();

	/**
	 * Called when the current ammo count changes.
	 * @param CurrentAmmo - The current ammo count.
	 * @param MagSize - The size of the magazine.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCurrentAmmoChanged(int32 CurrentAmmo, int32 MagSize);

	/**
	 * Called when the shoot sequence ends, allowing for cleanup or state reset.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEndShootSequence();

	/**
	 * Called when the last round is fired and the magazine is depleted.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMagEmpty();

	/**
	 * Gets the payload data for the reload event.
	 * @return The reload event data containing ammo type and amount.
	 */
	FReloadEventData GetReloadPayload() const;
	
	virtual bool NativeDeployWeaponAttack() override;
	
	virtual void OnEndWeaponAttack_Implementation() override;

private:
	/**
	 * Binds events related to the weapon's functionalities.
	 */
	void BindEvents();

	/**
	 * Unbinds events related to the weapon's functionalities.
	 */
	void UnbindEvents();

	/**
	 * Sets the payload for the reload event.
	 * @param InReloadPayload - The reload event data containing ammo type and amount.
	 */
	void SetReloadPayload(const FReloadEventData InReloadPayload);

	/**
	 * Reloads the weapon by inserting ammo into the magazine.
	 */
	void ReloadInsertAmmo();

	/**
	 * Called when the reload montage ends.
	 * @param bInterrupted - True if the montage was interrupted, false if it completed normally.
	 */
	UFUNCTION()
	void OnReloadMontageEnded(const bool bInterrupted);

	/**
	 * Sets the aim spread modifier based on the weapon's aim precision increase.
	 */
	void SetAimSpreadModifier();

	/**
	 * Sets the aim recoil modifier based on the weapon's aim recoil control increase.
	 */
	void SetAimRecoilModifier();

	/**
	 * Attaches the shoot barrel to the specified socket on the weapon mesh.
	 */
	void AttachBarrelToSocket() const;

	/**
	 * Checks if the given ammo type matches the weapon's current ammo type.
	 * @param AmmoData - Pointer to the ammo type data to compare against.
	 * @return True if the ammo types match, false otherwise.
	 */
	bool IsSameAmmoType(const UAmmoTypeData* AmmoData) const;

	/**
	 * Resets the recoil of the weapon to its default strength.
	 */
	void ResetAimWeaponRecoil() const;

	/**
	 * Resets the aim weapon spread to its default value.
	 */
	void ResetAimWeaponSpread() const;
};
