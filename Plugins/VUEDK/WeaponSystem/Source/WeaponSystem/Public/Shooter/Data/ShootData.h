// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootType.h"
#include "Curves/CurveVector.h"
#include "Engine/DataAsset.h"
#include "ShootData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FShootData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0", ToolTip = "rounds/min"))
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EShootType ShootType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MaxRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	int32 MagSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float RecoilStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float RecoilDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides,
		ToolTip = "The Y (vertical recoil) and Z (horizontal recoil) graphs define recoil strength per shot (horizontal axis = shots, vertical axis = strength), while the X graph controls recoil decay over time."))
	UCurveVector* RecoilCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "360", UIMax = "360"))
	float MaxSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "360", UIMax = "360"))
	float DefaultSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float SpreadChangeRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float SpreadRecoveryRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasSpread", EditConditionHides,
		ToolTip = "The curve defines the spread over time. The horizontal axis represents the shot index, while the vertical axis represents the spread value."))
	UCurveFloat* SpreadCurve;

	FShootData(): Damage(0),
	              FireRate(0),
	              ShootType(),
	              MaxRange(0),
	              MagSize(0),
	              bHasRecoil(false),
	              RecoilStrength(1.f),
	              RecoilDuration(.1f),
	              RecoilCurve(nullptr),
	              bHasSpread(false),
	              MaxSpread(10.f),
	              DefaultSpread(0.f),
	              SpreadChangeRate(15.f),
	              SpreadRecoveryRate(15.f),
	              SpreadCurve(nullptr)
	{
	}
};
