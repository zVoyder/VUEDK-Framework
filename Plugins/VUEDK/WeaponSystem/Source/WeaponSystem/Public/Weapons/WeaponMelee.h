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

	UFUNCTION(BlueprintPure)
	bool IsWeaponBlocking() const;

	UFUNCTION(BlueprintNativeEvent)
	bool CanDefend() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool StartDefense() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool StopDefense();

	UFUNCTION(BlueprintCallable)
	void EnableDamageHitbox();

	UFUNCTION(BlueprintCallable)
	void DisableDamageHitbox();

	void SetBlockActive(const bool bActive);

protected:
	virtual bool NativeDeployWeaponAttack() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackInterrupted();

private:
	void TraceDamageHitbox();

	void HandleHitActors(TArray<FHitResult> HitResults);

	void ClearHitActors();
};
