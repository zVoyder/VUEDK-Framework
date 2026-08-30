// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Data/WeaponMeleeData.h"
#include "Hitbox/MeleeHitboxesManager.h"
#include "Montages/MeleeMontagesManager.h"
#include "WeaponMelee.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWeaponComboStarted,
	UWeaponMeleeAttackData*, AttackData
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWeaponComboEnded,
	UWeaponMeleeAttackData*, AttackData
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWeaponAttackStarted,
	UWeaponMeleeAttackData*, AttackData,
	int32, AttackIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnWeaponAttackHit,
	UMeleeHitbox*, Hitbox,
	FHitResult, HitResult,
	float, Damage
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWeaponAttackInterrupted,
	UMeleeHitbox*, Hitbox,
	FHitResult, HitResult
);

UCLASS()
class WEAPONSYSTEM_API AWeaponMelee : public AWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponComboStarted OnWeaponComboStart;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponComboEnded OnWeaponComboEnd;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackStarted OnWeaponAttackStart;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackHit OnWeaponAttackPreHit;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackHit OnWeaponAttackHit;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackInterrupted OnWeaponAttackInterrupt;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMeleeHitboxesManager* MeleeHitboxesManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMeleeMontagesManager* MeleeMontagesManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponMeleeData WeaponMeleeData;

private:
	bool bIsBlockActive = false;
	UPROPERTY()
	TSet<AActor*> ActorsCurrentlyInHitbox;

public:
	AWeaponMelee();

	virtual UWeaponMontagesManagerBase* GetMontagesManager() const override;
	
	virtual bool IsWeaponAttacking() const override;

	/**
	 * Checks if the weapon is currently blocking.
	 * @return true if blocking, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsWeaponBlocking() const;

	/**
	 * Checks if the weapon can defend (can be overridden in Blueprints).
	 * @return true if the weapon can defend, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanDefend() const;

	/**
	 * Starts the defense action for the weapon.
	 * @return true if defense started successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool StartDefense() const;

	/**
	 * Stops the defense action for the weapon.
	 * @return true if defense stopped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool StopDefense();

	/**
	 * Enables the damage hitbox for the weapon.
	 */
	UFUNCTION(BlueprintCallable)
	void EnableDamageHitbox() const;

	/**
	 * Disables the damage hitbox for the weapon.
	 */
	UFUNCTION(BlueprintCallable)
	void DisableDamageHitbox() const;

	/**
	 * Sets the block state of the weapon.
	 * @param bActive True to activate block, false to deactivate.
	 */
	void SetBlockActive(const bool bActive);

	/** 
	 * Gets the current attack montage data.
	 * @return The current UWeaponMeleeAttackMontageData instance.
	 */
	UFUNCTION(BlueprintPure)
	UWeaponMeleeAttackData* GetCurrentAttack() const;

	UFUNCTION(BlueprintCallable)
	void SetAttackSpeedMultiplier(const float NewMultiplier) const;

	UFUNCTION(BlueprintCallable)
	void SetDefenseSpeedMultiplier(const float NewMultiplier) const;

	UFUNCTION(BlueprintCallable)
	void SetInterruptSpeedMultiplier(const float NewMultiplier) const;

	UFUNCTION(BlueprintPure)
	float GetAttackSpeedMultiplier() const;

	UFUNCTION(BlueprintPure)
	float GetDefenseSpeedMultiplier() const;

	UFUNCTION(BlueprintPure)
	float GetInterruptSpeedMultiplier() const;

	void CallComboStartedEvent(UWeaponMeleeAttackData* AttackData);

	void CallComboEndedEvent(UWeaponMeleeAttackData* AttackData);
	
	void CallAttackStartedEvent(UWeaponMeleeAttackData* AttackData, int32 AttackIndex);

	void CallPreHitEvent(UMeleeHitbox* Hitbox, const FHitResult& HitResult, float Damage);
	
	void CallHitEvent(UMeleeHitbox* Hitbox, const FHitResult& HitResult, float Damage);

	void CallInterruptEvent(UMeleeHitbox* Hitbox, const FHitResult& HitResult);

protected:
	virtual void BeginPlay() override;

	virtual bool NativeDeployWeaponAttack() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnComboStarted(UWeaponMeleeAttackData* AttackData);

	UFUNCTION(BlueprintNativeEvent)
	void OnComboEnded(UWeaponMeleeAttackData* AttackData);

	UFUNCTION(BlueprintNativeEvent)
	void OnAttackStarted(UWeaponMeleeAttackData* AttackData, int32 AttackIndex);

	UFUNCTION(BlueprintNativeEvent)
	void OnAttackPreHit(UMeleeHitbox* Hitbox, FHitResult HitResult, float Damage);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAttackHit(UMeleeHitbox* Hitbox, FHitResult HitResult, float Damage);

	UFUNCTION(BlueprintNativeEvent)
	void OnAttackInterrupted(UMeleeHitbox* Hitbox, FHitResult HitResult);
};
