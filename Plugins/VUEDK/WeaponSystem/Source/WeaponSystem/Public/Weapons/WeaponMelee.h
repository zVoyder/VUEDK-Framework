// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Data/WeaponMeleeData.h"
#include "Montages/MeleeMontagesManager.h"
#include "WeaponMelee.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackInterrupt
);

UCLASS()
class WEAPONSYSTEM_API AWeaponMelee : public AWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackInterrupt OnWeaponAttackInterrupt;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FWeaponMeleeData WeaponMeleeData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMeleeMontagesManager* MeleeMontagesManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* DamageHitboxPreview;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug = false;
#endif

private:
	bool bIsHitboxEnabled = false;
	bool bIsBlockActive = false;
	UPROPERTY()
	TSet<AActor*> ActorsCurrentlyInHitbox;

public:
	AWeaponMelee();
	
	virtual void Tick(float DeltaSeconds) override;
	
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
	void EnableDamageHitbox();

	/**
	 * Disables the damage hitbox for the weapon.
	 */
	UFUNCTION(BlueprintCallable)
	void DisableDamageHitbox();

	/**
	 * Sets the block state of the weapon.
	 * @param bActive - True to activate block, false to deactivate.
	 */
	void SetBlockActive(const bool bActive);

protected:
	virtual bool NativeDeployWeaponAttack() override;

	/**
	 * Called when the weapon attack is interrupted (can be overridden in Blueprints).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackInterrupted();

private:
	/**
	 * Traces the damage hitbox to detect hits.
	 */
	void TraceDamageHitbox();

	/**
	 * Handles the actors hit by the damage hitbox.
	 * @param HitResults - The array of hit results.
	 */
	void HandleHitActors(TArray<FHitResult> HitResults);

	/**
	 * Clears the list of hit actors.
	 */
	void ClearHitActors();
};
