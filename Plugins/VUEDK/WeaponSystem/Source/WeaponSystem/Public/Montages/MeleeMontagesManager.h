// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponMontagesManagerBase.h"
#include "Data/WeaponMeleeAttackMontageData.h"
#include "MeleeMontagesManager.generated.h"

class AWeaponMelee;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UMeleeMontagesManager : public UWeaponMontagesManagerBase
{
	GENERATED_BODY()

public:
	// -- Montages --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Defense", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DefenseCooldown = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Defense")
	FWeaponMontageData DefensiveMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attacks")
	FAlphaBlendArgs StartBlendIn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attacks")
	FAlphaBlendArgs StopBlendOut;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attacks")
	TArray<FWeaponMeleeAttackMontageData> AttackMontages;

private:
	UPROPERTY()
	AWeaponMelee* WeaponMelee;
	int32 CurrentAttackIndex = 0;
	bool bWantsToAttack = false;
	bool bWantsToDefend = false;
	bool bIsDefenseInCooldown = false;
	bool bHasBufferedAttack = false;
	bool bIsAttacking = false;
	bool bIsDefending = false;
	bool bEndAttack = false;
	bool bIsInterrupting = false;
	FTimerHandle DefenseCooldownTimer;
	FWeaponMeleeAttackMontageData* CurrentAttackMontage = nullptr;

public:
	UMeleeMontagesManager();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool IsBusy_Implementation() const override;

	/**
	 * Gets the current attack index.
	 * @return The index of the current attack in the attack montage array.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetAttackIndex() const;

	/**
	 * Checks if the current montage is interrupting.
	 * @return True if the montage is interrupting, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsMontageInterrupting() const;

	/**
	 * Checks if the current montage is attacking.
	 * @return True if the montage is attacking, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsMontageDefending() const;

	/**
	 * Checks if the current montage is attacking.
	 * @return True if the montage is attacking, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsMontageAttacking() const;

	/**
	 * Sets the weapon defensive state.
	 * @param bWants - True if the weapon wants to defend, false otherwise.
	 */
	void SetWantsToDefend(const bool bWants);

	/**
	 * Checks the the buffered attack state.
	 */
	void CheckBufferAttack();

	/**
	 * Clears the buffered attack state.
	 */
	void ClearBufferedAttack();
	
	/**
	 * Called when an attack begins (animation notify or logic event).
	 */
	void OnAttackBeginNotify();

	/**
	 * Called when an attack finishes (animation notify or logic event).
	 */
	void OnAttackFinishedNotify();

protected:
	/**
	 * Called when the component begins play. Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Called when a weapon attack succeeds.
	 */
	virtual void OnWeaponAttackSuccess() override;

	/**
	 * Called when a weapon attack ends.
	 */
	virtual void OnWeaponEndAttack() override;

	/**
	 * Called when a weapon attack is interrupted.
	 */
	UFUNCTION()
	void OnWeaponAttackInterrupted();

	/**
	 * Called when an attack interrupt finishes.
	 * @param bInterrupted - True if the attack was interrupted, false otherwise.
	 */
	UFUNCTION()
	void OnAttackInterruptFinished(bool bInterrupted);

	/**
	 * Starts a combo attack sequence.
	 */
	void StartComboAttack();

	/**
	 * Ends the current combo attack sequence.
	 */
	void EndComboAttack() const;

	/**
	 * Resets the combo attack state.
	 */
	void ResetComboAttack();

	/**
	 * Plays the next attack montage in the combo sequence.
	 */
	void PlayNextAttackMontage();

	/**
	 * Plays the attack montage at the specified index in the combo sequence.
	 * @param AttackIndex - The index of the attack montage to play.
	 */
	void PlayAttackMontageAt(const int32 AttackIndex);

	/**
	 * Plays the specified attack montage.
	 * @param AttackMontage - The attack montage data to play.
	 */
	void PlayAttackMontage(FWeaponMeleeAttackMontageData& AttackMontage);

	/**
	 * Ends the current attack sequence.
	 */
	void EndAttackSequence();

	/**
	 * Plays the defensive montage.
	 */
	void PlayDefensiveMontage();

	/**
	 * Stops the defensive montage.
	 */
	void StopDefensiveMontage();

	/**
	 * Processes the defense logic (e.g., parry, block).
	 */
	void ProcessDefense();

	/**
	 * Starts the defense cooldown timer.
	 */
	void StartDefenseCooldownTimer();

	/**
	 * Resets the defense cooldown state.
	 */
	UFUNCTION()
	void ResetDefenseCooldown();
};
