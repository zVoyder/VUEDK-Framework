// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MontageEndPriority.h"
#include "WeaponMontageData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnMontageBegin
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnMontageFinished,
	bool, bInterrupted
);

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FWeaponMontageData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMontageBegin OnMontageBegin;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMontageFinished OnMontageFinished;
	FOnMontageEnded OnMontageEndedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	TSoftObjectPtr<UAnimMontage> WeaponMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	bool bWeaponMontageStopAllMontages = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	TSoftObjectPtr<UAnimMontage> CharacterMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	bool bCharacterMontageStopAllMontages = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	TEnumAsByte<EMontageEndPriority> MontageEndPriority;

	FWeaponMontageData(): WeaponMontage(nullptr),
	                      CharacterMontage(nullptr),
	                      MontageEndPriority()
	{
	}

	FWeaponMontageData(UAnimMontage* InWeaponMontage, UAnimMontage* InCharacterMontage): WeaponMontage(InWeaponMontage),
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
};
