// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MontageEndPriority.h"
#include "WeaponMontageData.generated.h"

struct FWeaponMontageData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponMontageBegin
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWeaponMontageFinished,
	bool, bInterrupted
);

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FWeaponMontageData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponMontageBegin OnWeaponMontageBegin;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponMontageFinished OnWeaponMontageFinished;
	
	FOnMontageEnded OnMontageEndedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
	TSoftObjectPtr<UAnimMontage> WeaponMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
	TSoftObjectPtr<UAnimMontage> CharacterMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages|Advanced")
	bool bWeaponMontageStopAllMontages = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages|Advanced")
	bool bCharacterMontageStopAllMontages = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages|Advanced")
	TEnumAsByte<EMontageEndPriority> MontageEndPriority;

	FWeaponMontageData() : WeaponMontage(nullptr),
	                       CharacterMontage(nullptr),
	                       MontageEndPriority()
	{
	}

	FWeaponMontageData(UAnimMontage* InWeaponMontage, UAnimMontage* InCharacterMontage) : WeaponMontage(InWeaponMontage),
	                                                                                      CharacterMontage(InCharacterMontage),
	                                                                                      MontageEndPriority()
	{
	}

	UAnimMontage* GetWeaponMontage() const
	{
		return WeaponMontage.LoadSynchronous();
	}

	UAnimMontage* GetCharacterMontage() const
	{
		return CharacterMontage.LoadSynchronous();
	}

	bool operator==(const FWeaponMontageData& ShootMontage) const
	{
		return WeaponMontage == ShootMontage.WeaponMontage && CharacterMontage == ShootMontage.CharacterMontage;
	}
};
