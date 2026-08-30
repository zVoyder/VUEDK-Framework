// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ToolTip = "If true, the weapon will automatically initialize on BeginPlay if it has a valid owner pawn."))
	bool bAutoInitialize = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float Damage;
	
	FWeaponData(): Damage(0.0f)
	{
	}
};
