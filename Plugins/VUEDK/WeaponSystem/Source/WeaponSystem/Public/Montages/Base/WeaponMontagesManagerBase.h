// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Components/ActorComponent.h"
#include "WeaponMontagesManagerBase.generated.h"

UCLASS(Abstract, NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UWeaponMontagesManagerBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	FWeaponMontageData EquipMontageData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	FWeaponMontageData UnequipMontageData;

protected:
	UPROPERTY()
	AWeaponBase* Weapon;

private:
	bool bIsPlayingEquipMontage;

public:
	UWeaponMontagesManagerBase();

	void ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	void PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	void StartWeaponMontage(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate) const;
	
	void StartWeaponMontageWithBlends(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn) const;

	void StopWeaponMontage(const FWeaponMontageData WeaponMontageData) const;
	
	void StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut) const;
	
	bool IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool IsBusy() const;
	
	UFUNCTION(BlueprintPure)
	bool IsEquipOrUnequipMontagePlaying() const;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void BeginPlay() override;

	virtual bool Check() const;

	UFUNCTION()
	virtual void OnWeaponAttackSuccess();

	UFUNCTION()
	virtual void OnWeaponAttackFail();

	UFUNCTION()
	virtual void OnWeaponEndAttack();

private:
	void StartEquipMontage();
	
	void StartUnequipMontage();
	
	UFUNCTION()
	void OnWeaponEquipped();

	UFUNCTION()
	void OnWeaponUnequipped();

	UFUNCTION()
	void OnWeaponReadyToUse(bool bInterrupted);
};
