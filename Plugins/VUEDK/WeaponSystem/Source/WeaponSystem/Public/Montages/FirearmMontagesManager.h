// Copyright zVoyder, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/WeaponMontagesManagerBase.h"
#include "FirearmMontagesManager.generated.h"

class AWeaponFirearm;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UFirearmMontagesManager : public UWeaponMontagesManagerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData ReloadMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData ShootMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Actions")
	FWeaponMontageData FailShootMontage;

protected:
	UPROPERTY()
	AWeaponFirearm* WeaponFirearm;

public:
	UFirearmMontagesManager();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	bool IsAnimShooting() const;

	UFUNCTION()
	bool IsAnimFailShooting() const;

protected:
	virtual void BeginPlay() override;

	virtual bool Check() const override;

private:
	virtual void OnWeaponAttackSuccess() override;

	virtual void OnWeaponAttackFail() override;

	void GetShootPlayRates(const FWeaponMontageData& WeaponMontageData, float& WeaponPlayRate, float& CharacterPlayRate) const;
};
