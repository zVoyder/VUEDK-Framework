// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FWeaponData
{
	GENERATED_BODY()

	// -- Information --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info")
	FName WeaponName;

	// -- Stats --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float Damage;
	
	FWeaponData(): Damage(0.0f)
	{
	}
};
