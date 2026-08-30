// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Montages/Data/WeaponMontageData.h"
#include "WeaponMontagesManagerBase.generated.h"

class AWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyWeaponMontageBegin,
	const FWeaponMontageData&, WeaponMontageData
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAnyWeaponMontageFinished,
	const FWeaponMontageData&, WeaponMontageData,
	bool, bInterrupted
);

UCLASS(Abstract, NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UWeaponMontagesManagerBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyWeaponMontageBegin OnAnyWeaponMontageBegin;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyWeaponMontageFinished OnAnyWeaponMontageFinished;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData EquipMontageData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData UnequipMontageData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Advanced")
	bool bUseTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Advanced", meta = (ToolTip = "Used to find the anim instance of the owner's mesh if it is not a character."))
	FName AnimInstanceMeshTag = TEXT("AnimInstanceMesh"); // Use a tag to be more flexible
	
protected:
	UPROPERTY()
	TMap<UAnimMontage*, FWeaponMontageData> PlayingMontages;
	UPROPERTY()
	AWeaponBase* Weapon;
	UPROPERTY()
	UAnimInstance* OwnerAnimInstance;

private:
	bool bIsPlayingEquipMontage;

public:
	UWeaponMontagesManagerBase();

	void Init(AWeaponBase* InWeapon);

	void ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	void PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	void StartWeaponMontage(FWeaponMontageData& WeaponMontageData, float WeaponPlayRate, float CharacterPlayRate);
	
	void StartWeaponMontageWithBlends(FWeaponMontageData& WeaponMontageData, float WeaponPlayRate, float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn);

	void StopWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;

	void StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut) const;

	bool IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;

	/**
	 * Checks if the manager is currently busy (e.g., playing a montage).
	 * @return true if busy, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool IsBusy() const;

	/**
	 * Checks if an equip or unequip montage is currently playing.
	 * @return true if an equip or unequip montage is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEquipOrUnequipMontagePlaying() const;
	
	UFUNCTION(BlueprintPure)
	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION(BlueprintPure)
	UAnimInstance* GetWeaponAnimInstance() const;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupWeapons(AWeaponBase* InWeapon);
	
	virtual void BindEvents();

	virtual void UnbindEvents();

	UFUNCTION()
	virtual void OnWeaponAttackSuccess();

	UFUNCTION()
	virtual void OnWeaponAttackFail();

	UFUNCTION()
	virtual void OnWeaponEndAttack();

	virtual void OnWeaponMontageBegin(const FWeaponMontageData& WeaponMontageData);

	virtual void OnWeaponMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted);

	virtual bool Check() const;

private:
	void AddPlayingMontage(UAnimMontage* Montage, const FWeaponMontageData& WeaponMontageData);
	
	void RemovePlayingMontage(const UAnimMontage* Montage);

	void PlayMontageInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, bool bRegisterPlayingMontage = true);

	void PlayMontageWithBlendInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, const FAlphaBlendArgs& BlendIn, bool bRegisterPlayingMontage = true);

	void RegisterMontageBegin(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage);
	
	void SetOwnerAnimInstance();
	
	void SetWeaponMetaData(UAnimMontage* Montage) const;
	
	void StartEquipMontage();
	
	void StartUnequipMontage();
	
	UFUNCTION()
	void OnWeaponEquipped();

	UFUNCTION()
	void OnWeaponUnequipped();

	UFUNCTION()
	void OnWeaponReadyToUse(bool bInterrupted);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
};
