// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "DismemberableLimbData.generated.h"

USTRUCT(BlueprintType)
struct FDismemberableLimbData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BoneName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanExplode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bCanExplode", EditConditionHides, ToolTip = "The damage threshold required to explode the limb."))
	float DamageToExplode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bCanExplode", EditConditionHides, ToolTip = "The Niagara system to spawn when the limb explodes."))
	UNiagaraSystem* LimbExplosionFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LimbMass;
	
	FDismemberableLimbData(): bCanExplode(false),
	                          DamageToExplode(0.f),
	                          LimbExplosionFX(nullptr),
	                          LimbMass(1.f)
	{
	}

	explicit FDismemberableLimbData(const FName InBoneName)
		: BoneName(InBoneName),
		  bCanExplode(false),
		  DamageToExplode(0.f),
		  LimbExplosionFX(nullptr),
		  LimbMass(1.f)
	{
	}

	bool operator==(const FDismemberableLimbData& Other) const
	{
		return BoneName == Other.BoneName;
	}

	bool IsGoingToExplode(const float Damage) const
	{
		return bCanExplode && Damage > DamageToExplode;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDismemberableLimbData& LimbData)
{
	return GetTypeHash(LimbData.BoneName);
}

