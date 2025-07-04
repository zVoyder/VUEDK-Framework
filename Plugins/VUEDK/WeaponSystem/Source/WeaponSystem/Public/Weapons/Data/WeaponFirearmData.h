// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmmoTypeData.h"
#include "Engine/DataAsset.h"
#include "WeaponFirearmData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FWeaponFirearmData
{
	GENERATED_BODY()

public:
	// -- Stats --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "100.0", UIMax = "100.0"), Category = "Weapon|Stats")
	float AimPrecisionIncrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "100.0", UIMax = "100.0"), Category = "Weapon|Stats")
	float AimRecoilControlIncrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Weapon|Stats")
	float SpreadChangeRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ReloadTime;

	// -- Ammo --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	UAmmoTypeData* AmmoType;

	FWeaponFirearmData(): AimPrecisionIncrease(0),
	                      AimRecoilControlIncrease(0),
	                      SpreadChangeRate(25.f),
	                      ReloadTime(2.0f),
	                      AmmoType(nullptr)
	{
	}
};
