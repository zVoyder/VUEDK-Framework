// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMontageData.h"
#include "WeaponMeleeAttackMontageData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponMeleeAttackMontageData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	FWeaponMontageData AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	FWeaponMontageData AttackInterruptMontage;
};
