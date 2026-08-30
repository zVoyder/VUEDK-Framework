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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "100.0", UIMax = "100.0"))
	float AimPrecisionIncrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "100.0", UIMax = "100.0"))
	float AimRecoilControlIncrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SpreadChangeRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of the socket where the barrel component is attached. If not found, the root component will be used."))
	FName ShootBarrelSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanDeployAttackIfReloading = false;

	FWeaponFirearmData(): AimPrecisionIncrease(0),
	                      AimRecoilControlIncrease(0),
	                      SpreadChangeRate(25.f),
	                      ReloadTime(2.0f)
	{
	}
};
