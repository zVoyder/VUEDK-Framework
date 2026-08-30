// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shooter/Data/ShootData.h"
#include "WeaponShootData.generated.h"

/**
 * This asset is intended to be used with a ShooterBehaviour tied to a weapon.
 * If you don't need using a UWeaponFirearm, you can use UShootData directly for your behaviours.
 */
UCLASS()
class WEAPONSYSTEM_API UWeaponShootData : public UShootData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages", meta = (EditCondition = "bEnableShootSuccessMontage", ToolTip = "Index of the shoot montage to play when shooting successfully."))
	int32 ShootSuccessMontageIndex = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages", meta = (EditCondition = "bEnableShootFailMontage", ToolTip = "Index of the fail shoot montage to play when shooting fails."))
	int32 ShootFailMontageIndex = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages", meta = (EditCondition = "bEnableReloadMontage", ToolTip = "Index of the reload montage to play."))
	int32 ReloadMontageIndex = 0;

	UPROPERTY()
	bool bEnableShootSuccessMontage = true;
	UPROPERTY()
	bool bEnableShootFailMontage = true;
	UPROPERTY()
	bool bEnableReloadMontage = true;
};
