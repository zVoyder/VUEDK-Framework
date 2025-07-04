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
	
	UFUNCTION(BlueprintPure)
	int32 GetAttackIndex() const;
	
	UFUNCTION(BlueprintPure)
	bool IsMontageInterrupting() const;

	UFUNCTION(BlueprintPure)
	bool IsMontageDefending() const;

	UFUNCTION(BlueprintPure)
	bool IsMontageAttacking() const;

	void SetWantsToDefend(const bool bWants);

	void CheckBufferAttack();

	void ClearBufferedAttack();
	
	void OnAttackBeginNotify();
	
	void OnAttackFinishedNotify();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnWeaponAttackSuccess() override;

	virtual void OnWeaponEndAttack() override;
	
	UFUNCTION()
	void OnWeaponAttackInterrupted();

	UFUNCTION()
	void OnAttackInterruptFinished(bool bInterrupted);

	void StartComboAttack();

	void EndComboAttack() const;

	void ResetComboAttack();

	void PlayNextAttackMontage();

	void PlayAttackMontageAt(const int32 AttackIndex);

	void PlayAttackMontage(FWeaponMeleeAttackMontageData& AttackMontage);

	void EndAttackSequence();

	void PlayDefensiveMontage();

	void StopDefensiveMontage();

	void ProcessDefense();

	void StartDefenseCooldownTimer();

	UFUNCTION()
	void ResetDefenseCooldown();
};
