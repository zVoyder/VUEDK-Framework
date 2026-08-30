// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponData.h"
#include "GameFramework/Actor.h"
#include "Montages/Base/WeaponMontagesManagerBase.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackFailed
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackEnded
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponEquipped
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponUnequipped
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class WEAPONSYSTEM_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackSuccess OnWeaponAttacked;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackFailed OnWeaponAttackFailed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackEnded OnWeaponAttackEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponEquipped OnWeaponEquippedEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponUnequipped OnWeaponUnequippedEvent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponData WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* WeaponRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* WeaponMesh;

private:
	UPROPERTY()
	UObject* Payload;
	bool bIsEquipped = false;
	bool bIsWeaponAttacking = false;

public:
	AWeaponBase();

	/**
	 * @brief Initialize the weapon with an owner and optional payload.
	 * @param InOwner The pawn that owns this weapon.
	 * @param InPayload Optional payload object to associate with the weapon.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr);

	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetWeaponMesh() const;

	/**
	 * @brief Sets the weapon in the equipped state.
	 * (NOTE: This is not called internally, it is useful for integration with external systems and is intended to be called externally).
	 */
	UFUNCTION(BlueprintCallable)
	void Equip();

	/**
	 * @brief Sets the weapon in the unequipped state.
	 * (NOTE: This is not called internally, it is useful for integration with external systems and is intended to be called externally).
	 */
	UFUNCTION(BlueprintCallable)
	void Unequip();

	/**
     * Performs a weapon attack action.
     * @return true if the attack was started successfully, false otherwise.
     */
	UFUNCTION(BlueprintCallable)
	bool WeaponAttack();

	/**
	 * Ends the current weapon attack action.
	 */
	UFUNCTION(BlueprintCallable)
	void EndWeaponAttack();

	/**
	 * Sets the payload object associated with this weapon.
	 * @param InPayload The new payload object.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPayload(UObject* InPayload);

	/**
	 * Sets the weapon's damage value.
	 * @param NewDamage The new damage value to set.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponDamage(const float NewDamage);

	/**
	 * Returns the current weapon damage value.
	 * @return The current damage value of the weapon.
	 */
	UFUNCTION(BlueprintPure)
	float GetWeaponDamage() const;

	/**
	 * Returns the payload object associated with this weapon.
	 * @return The payload UObject pointer, or nullptr if not set.
	 */
	UFUNCTION(BlueprintPure)
	UObject* GetPayload() const;

	/**
     * Checks if the weapon is currently equipped.
     * @return true if the weapon is equipped, false otherwise.
     */
	UFUNCTION(BlueprintPure)
	bool IsEquipped() const;

	/**
	 * Checks if the weapon is currently performing an attack.
	 * @return true if the weapon is attacking, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	virtual bool IsWeaponAttacking() const;

	/**
	 * Returns the weapon data struct containing all relevant weapon properties.
	 * @return The FWeaponData struct for this weapon.
	 */
	UFUNCTION(BlueprintPure)
	FWeaponData GetWeaponData() const;

	UFUNCTION(BlueprintPure)
	virtual UWeaponMontagesManagerBase* GetMontagesManager() const;

protected:
	virtual void BeginPlay() override;

	/**
	 * Checks if the weapon can deploy an attack.
	 * @return True if the weapon can deploy an attack, false otherwise.
	 */
	virtual bool NativeDeployWeaponAttack();

	/**
	 * Called to deploy a weapon attack.
	 * @return true if the attack was deployed successfully, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool DeployWeaponAttack();

	/**
	 * Called when a weapon attack ends.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEndWeaponAttack();

	/**
	 * Called when a weapon attack is successful.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackSuccess();

	/**
	 * Called when a weapon attack fails.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackFail();

	/**
	 * Called when the weapon is equipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponEquipped();

	/**
	 * Called when the weapon is unequipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponUnequipped();
};
