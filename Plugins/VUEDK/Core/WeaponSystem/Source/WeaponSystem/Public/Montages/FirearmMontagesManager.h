// Copyright zVoyder, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponMontagesManagerBase.h"
#include "Data/WeaponMontageData.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"
#include "FirearmMontagesManager.generated.h"

class AWeaponFirearm;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnFirearmMontagePlayed,
	const FWeaponMontageData&, WeaponMontageData,
	int32, MontageIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnFirearmMontageFinished,
	const FWeaponMontageData&, WeaponMontageData,
	int32, MontageIndex,
	bool, bInterrupted
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UFirearmMontagesManager : public UWeaponMontagesManagerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFirearmMontagePlayed OnShootMontagePlayed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFirearmMontagePlayed OnShootFailMontagePlayed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFirearmMontagePlayed OnReloadMontagePlayed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFirearmMontageFinished OnShootMontageEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFirearmMontageFinished OnShootFailMontageEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFirearmMontageFinished OnReloadMontageEnded;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions|Shoot")
	TArray<FWeaponMontageData> ShootSuccessMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions|Shoot")
	TArray<FWeaponMontageData> ShootFailMontages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions|Reload")
	TArray<FWeaponMontageData> ReloadMontages;

protected:
	UPROPERTY()
	AWeaponFirearm* WeaponFirearm;

public:
	UFirearmMontagesManager();
	
	UFUNCTION(BlueprintCallable)
	bool TryPlayReloadMontage(const int32 MontageIndex = 0);
	
	UFUNCTION(BlueprintCallable)
	bool TryPlayShootMontage(const int32 MontageIndex = 0);

	UFUNCTION(BlueprintCallable)
	bool TryPlayShootFailMontage(const int32 MontageIndex = 0);
	
	UFUNCTION(BlueprintCallable)
	void StopShootMontage(const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut);

	UFUNCTION(BlueprintCallable)
	void StopShootFailMontage(const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut);
	
	UFUNCTION(BlueprintCallable)
	void StopReloadMontage(const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut);

	/**
	 * Checks if a shooting animation is currently playing.
	 * @return true if the shooting animation is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimShooting() const;

	/**
	 * Checks if a fail shooting animation is currently playing.
	 * @return true if a fail shooting animation is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimShootFailing() const;

	/**
	 * Checks if a reloading animation is currently playing.
	 * @return true if a reloading animation is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnimReloading() const;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void SetupWeapons(AWeaponBase* InWeapon) override;
	
	virtual void BindEvents() override;

	virtual void UnbindEvents() override;
	
	virtual bool Check() const override;
	
	UFUNCTION()
	void OnBehaviourShootSuccess(UShooterBehaviourBase* Behaviour, UShootBarrel* ShootBarrel, int32 ShotIndex);

	UFUNCTION()
	void OnBehaviourShootFail(UShooterBehaviourBase* Behaviour, EShootFailReason FailReason);

	virtual void OnWeaponMontageBegin(const FWeaponMontageData& WeaponMontageData) override;

	virtual void OnWeaponMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) override;

private:
	void GetShootPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const;

	void GetReloadPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const;
	
	void HandleShootSuccessMontageBegin(const FWeaponMontageData& WeaponMontageData) const;

	void HandleShootFailMontageBegin(const FWeaponMontageData& WeaponMontageData) const;

	void HandleReloadMontageBegin(const FWeaponMontageData& WeaponMontageData) const;

	void HandleShootSuccessMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) const;

	void HandleShootFailMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) const;

	void HandleReloadMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted) const;
};
