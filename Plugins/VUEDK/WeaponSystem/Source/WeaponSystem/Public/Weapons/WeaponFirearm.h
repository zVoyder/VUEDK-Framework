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

	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr) override;

	UFUNCTION(BlueprintCallable)
	void AddWeaponDynamicSpread(const float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f) const;

	UFUNCTION(BlueprintCallable)
	void ResetCooldown() const;
	
	UFUNCTION(BlueprintCallable)
	void ResetSpread() const;

	virtual void SetWeaponDamage(const float NewDamage) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponFireRate(const float NewFireRate) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponMagSize(const int32 NewMagSize) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponMaxRange(const float NewRange) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponRecoilStrength(const float NewRecoilStrength) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponReloadTime(const float NewReloadTime);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponAmmoType(UAmmoTypeData* NewAmmoType);
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponShootType(const EShootType NewShootType) const;

	UFUNCTION(BlueprintCallable)
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = true) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponSpread(const float InSpread, const bool bOverrideDefault = true) const;

	UFUNCTION(BlueprintPure)
	FWeaponFirearmData GetWeaponFirearmData() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponFireRate() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponMagSize() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponMaxRange() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponRecoilStrength() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponReloadTime() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetNeededAmmo() const;

	UFUNCTION(BlueprintPure)
	UAmmoTypeData* GetWeaponAmmoType() const;

	UFUNCTION(BlueprintPure)
	EShootType GetWeaponShootType() const;

	UFUNCTION(BlueprintPure)
	float GetWeaponMaxSpread() const;
	
	UFUNCTION(BlueprintPure)
	float GetDefaultSpread() const;
	
	UFUNCTION(BlueprintPure)
	float GetWeaponSpread() const;

	UFUNCTION(BlueprintCallable)
	void ResetToDefaultShootType() const;

	UFUNCTION(BlueprintCallable)
	void ReloadWithMontage(UAmmoTypeData* AmmoData, const int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void InterruptReload(float CharacterBlendOutTime = 0.0f, float WeaponBlendOutTime = 0.0f);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 Refill(int32 Ammo) const;

	UFUNCTION(BlueprintCallable)
	void RefillAllMagazine() const;

	UFUNCTION(BlueprintCallable)
	void SetAim(bool bIsEnabled);

	UFUNCTION(BlueprintPure)
	bool IsAiming() const;

	UFUNCTION(BlueprintPure)
	bool HasJustShot() const;

	UFUNCTION(BlueprintPure)
	bool IsReloading() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void EnableAim();

	virtual void DisableAim();

	UFUNCTION(BlueprintNativeEvent)
	bool CanReload() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnReloadSuccess(float Remain, float ReloadedAmmo);

	UFUNCTION(BlueprintNativeEvent)
	void OnReloadFail();

	UFUNCTION(BlueprintNativeEvent)
	void OnEnableAim();

	UFUNCTION(BlueprintNativeEvent)
	void OnDisableAim();

	UFUNCTION(BlueprintNativeEvent)
	void OnCurrentAmmoChanged(int32 CurrentAmmo, int32 MagSize);

	UFUNCTION(BlueprintNativeEvent)
	void OnEndShootSequence();

	UFUNCTION(BlueprintNativeEvent)
	void OnMagEmpty();
	
	FReloadEventData GetReloadPayload() const;

	virtual bool NativeDeployWeaponAttack() override;

	virtual void OnEndWeaponAttack_Implementation() override;

private:
	void BindEvents();

	void UnbindEvents();

	void SetReloadPayload(const FReloadEventData InReloadPayload);

	void ReloadInsertAmmo();

	UFUNCTION()
	void OnReloadMontageEnded(const bool bInterrupted);

	void SetAimSpreadModifier();

	void SetAimRecoilModifier();

	void AttachBarrelToSocket() const;

	bool IsSameAmmoType(const UAmmoTypeData* AmmoData) const;

	void ResetAimWeaponRecoil() const;

	void ResetAimWeaponSpread() const;
};
