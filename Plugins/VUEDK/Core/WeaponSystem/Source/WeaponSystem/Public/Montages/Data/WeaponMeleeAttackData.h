// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMontageData.h"
#include "Engine/DataAsset.h"
#include "WeaponMeleeAttackData.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponMeleeAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponMontageData AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponMontageData AttackInterruptMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "If true, this attack can deal damage multiple times during its active window. Leaving and re-entering the hitbox will register additional hits."))
	bool bAllowsMultipleHits = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bAllowsMultipleHits", ToolTip = "Maximum number of hits allowed per attack. 0 means unlimited hits."))
	int32 MaxHits = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bAllowsMultipleHits", ClampMin = "0.0", UIMin = "0.0", ToolTip = "Time interval (in seconds) between consecutive hits when multiple hits are allowed. 0 means never."))
	float AttackTickRate = 0.0f;
};
