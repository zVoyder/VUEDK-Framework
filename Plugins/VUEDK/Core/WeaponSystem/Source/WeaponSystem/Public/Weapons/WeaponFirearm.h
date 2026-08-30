// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Data/ReloadEvent/ReloadEventData.h"
#include "Data/WeaponFirearmData.h"
#include "Montages/FirearmMontagesManager.h"
#include "Shooter/Shooter.h"
#include "WeaponFirearm.generated.h"

class UReloadManager;
class UShootBarrel;

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
	FOnAimEnabled OnAimEnabled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAimDisabled OnAimDisabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UFirearmMontagesManager* FirearmMontagesManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShootBarrel* MainShootBarrel;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponFirearmData WeaponFirearmData;

private:
	UPROPERTY()
	UReloadManager* ReloadManager;
	bool bIsAimingDownSight;
	bool bHasReloadInsertedAmmo;
	float AimSpread;
	float DefaultRecoilStrength;
	float AdsRecoilStrength;

public:
	AWeaponFirearm();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr) override;

	virtual UWeaponMontagesManagerBase* GetMontagesManager() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot(const int32 BehaviourIndex = 0);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void EndShootSequence(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	bool IsMagFull(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	bool IsMagEmpty(const int32 BehaviourIndex = 0) const;

	/**
	 * Adds dynamic spread to the weapon, affecting aim stability.
	 * @param AddSpread The amount of spread to add.
	 * @param ChangeRate The rate at which the spread changes.
	 * @param RecoveryRate The rate at which the spread recovers.
	 * @param BehaviourIndex The index of the shooter behaviour to apply the spread to.
	 */
	UFUNCTION(BlueprintCallable)
	void AddWeaponDynamicSpread(const float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void ResetCooldown(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void ResetSpread(const int32 BehaviourIndex = 0) const;

	virtual void SetWeaponDamage(const float NewDamage) override;

	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponFireRate(const float NewFireRate, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponMagazineSize(const int32 NewSize, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponMaxRange(const float NewRange, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponRecoilStrength(const float NewRecoilStrength, const int32 BehaviourIndex = 0) const;

	/**
	 * Sets the reload time of the weapon.
	 * @param NewReloadTime The new reload time.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeaponReloadTime(const float NewReloadTime);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponShootType(const EShootType NewShootType, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = true, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponSpread(const float InSpread, const bool bOverrideDefault = true, const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	UShooterBehaviourBase* GetShooterBehaviour(const int32 BehaviourIndex) const;

	UFUNCTION(BlueprintPure)
	UMagazine* GetWeaponMagazine(const int32 MagazineIndex = 0) const;

	/**
	 * Gets the firearm data structure containing various weapon properties.
	 * @return The weapon data structure.
	 */
	UFUNCTION(BlueprintPure)
	FWeaponFirearmData GetWeaponFirearmData() const;

	UFUNCTION(BlueprintPure)
	UReloadManager* GetReloadManager() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponFireRate(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	int32 GetWeaponMagSize(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	float GetWeaponMaxRange(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	float GetWeaponRecoilStrength(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	float GetWeaponReloadTime() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	int32 GetNeededAmmo(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	UAmmoTypeData* GetWeaponAmmoType(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	EShootType GetWeaponShootType(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	float GetWeaponMaxSpread(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	float GetDefaultSpread(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintPure)
	float GetWeaponSpread(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void ResetToDefaultShootType(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void Reload(const FReloadRequest& Request) const;

	UFUNCTION(BlueprintCallable)
	void FullReload(const EReloadMode ReloadMode = EReloadMode::Parallel) const;

	UFUNCTION(BlueprintCallable)
	void InterruptReload(const float CharacterBlendOutTime = 0.2f, const float WeaponBlendOutTime = 0.2f) const;

	UFUNCTION(BlueprintCallable)
	void SetAim(const bool bIsEnabled, const int32 BehaviourIndex = 0);
	
	/**
	 * Checks if the weapon is currently aiming.
	 * @return true if aiming, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAiming() const;

	/**
	 * Checks if the weapon is currently reloading.
	 * @return true if reloading, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsReloading() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool CanShoot();

	virtual bool NativeDeployWeaponAttack() override;

	virtual void OnEndWeaponAttack_Implementation() override;

	virtual void EnableAim(int32 BehaviourIndex = 0);

	virtual void DisableAim(int32 BehaviourIndex = 0);

	/**
	 * Checks if the weapon can reload.
	 * @return True if the weapon can reload, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanReload() const;

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

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourEnable(UShooterBehaviourBase* Behaviour);

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourDisable(UShooterBehaviourBase* Behaviour);

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourShootSuccess(UShooterBehaviourBase* Behaviour, UShootBarrel* ShootBarrel, int32 ShotIndex);

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourShootFail(UShooterBehaviourBase* Behaviour, EShootFailReason FailReason);

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourShootSequenceEnd(UShooterBehaviourBase* Behaviour);

	/**
	 * Called when the ammo count changes in a shooter behaviour.
	 * (NOTE: This is called locally for each behaviour, use "OnMagazine" events for global events)
	 * @param Behaviour The shooter behaviour where the ammo change occurred.
	 * @param Magazine The magazine associated with the behaviour.
	 * @param CurrentAmmo The current ammo count after the change.
	 * @param MagSize The total magazine size.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourAmmoChange(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize);

	/**
	 * Called when ammo is refilled in a shooter behaviour.
	 * (NOTE: This is called locally for each behaviour, use "OnMagazine" events for global events)
	 * @param Behaviour The shooter behaviour where the refill occurred.
	 * @param Magazine The magazine associated with the behaviour.
	 * @param CurrentAmmo The current ammo count after the refill.
	 * @param RefilledAmmo The amount of ammo that was refilled.
	 * @param RemainingAmmo The remaining ammo available for refilling.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourRefill(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);

	/**
	 * Called when the magazine in a shooter behaviour becomes full.
	 * (NOTE: This is called locally for each behaviour, use "OnMagazine" events for global events)
	 * @param Behaviour The shooter behaviour where the magazine became full.
	 * @param Magazine The magazine associated with the behaviour.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourFull(UShooterBehaviourBase* Behaviour, UMagazine* Magazine);

	/**
	 * Called when the magazine in a shooter behaviour becomes empty.
	 * (NOTE: This is called locally for each behaviour, use "OnMagazine" events for global events)
	 * @param Behaviour The shooter behaviour where the magazine became empty.
	 * @param Magazine The magazine associated with the behaviour.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourEmpty(UShooterBehaviourBase* Behaviour, UMagazine* Magazine);

	/**
	 * Called when ammo is refilled in the magazine.
	 * (NOTE: This is a global event for the magazine, use "OnBehaviour" events for behaviour-specific events)
	 * @param MagInstigator The object that initiated the magazine refill.
	 * @param Magazine The magazine that was refilled.
	 * @param CurrentAmmo The current ammo count after the refill.
	 * @param RefilledAmmo The amount of ammo that was refilled.
	 * @param RemainingAmmo The remaining ammo available for refilling.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMagazineRefill(const UObject* MagInstigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);

	/**
	 * Called when the ammo count changes in the magazine.
	 * (NOTE: This is a global event for the magazine, use "OnBehaviour" events for behaviour-specific events)
	 * @param MagInstigator The object that initiated the ammo change.
	 * @param Magazine The magazine where the ammo change occurred.
	 * @param CurrentAmmo The current ammo count after the change.
	 * @param MagSize The total magazine size.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMagazineAmmoChange(const UObject* MagInstigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize);

	/**
	 * Called when the magazine becomes full.
	 * (NOTE: This is a global event for the magazine, use "OnBehaviour" events for behaviour-specific events)
	 * @param MagInstigator The object that initiated the magazine full event.
	 * @param Magazine The magazine that became full.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMagazineFull(const UObject* MagInstigator, const UMagazine* Magazine);

	/**
	 * Called when the magazine becomes empty.
	 * (NOTE: This is a global event for the magazine, use "OnBehaviour" events for behaviour-specific events)
	 * @param MagInstigator The object that initiated the magazine empty event.
	 * @param Magazine The magazine that became empty.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMagazineEmpty(const UObject* MagInstigator, const UMagazine* Magazine);

	/**
	 * Called when a reload starts.
	 * @param Request The reload request data.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadStart(const FReloadRequest& Request);

	/**
	 * Called when a reload ends.
	 * @param Request The reload request data.
	 * @param bInterrupted Whether the reload was interrupted, if not the reload completed successfully.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadEnd(const FReloadRequest& Request, bool bInterrupted);

	/**
	 * Called when ammo is inserted during a reload.
	 * @param Behaviour The shooter behaviour where ammo was inserted.
	 * @param InsertedAmmo The amount of ammo that was inserted.
	 * @param RemainingAmmo The remaining ammo available after insertion.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadInsertAmmo(UShooterBehaviourBase* Behaviour, int32 InsertedAmmo, int32 RemainingAmmo);

private:
	void BindEvents();

	void UnbindEvents();

	void SetAimSpreadModifier(const int32 BehaviourIndex = 0);

	void SetAimRecoilModifier(const int32 BehaviourIndex = 0);

	void ResetAimWeaponRecoil(const int32 BehaviourIndex = 0) const;

	void ResetAimWeaponSpread(const int32 BehaviourIndex = 0) const;

	void AttachBarrelToSocket() const;
};
