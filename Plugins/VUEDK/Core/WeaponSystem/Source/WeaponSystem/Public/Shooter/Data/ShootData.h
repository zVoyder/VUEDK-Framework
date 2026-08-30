// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootType.h"
#include "Curves/CurveVector.h"
#include "Engine/DataAsset.h"
#include "Shooter/Behaviours/ShootModes/Base/ShootModeBase.h"
#include "ShootData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class WEAPONSYSTEM_API UShootData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	bool bHasInfiniteAmmo = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TEnumAsByte<ECollisionChannel> SightTraceChannel = ECollisionChannel::ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta = (ClampMin = 0, UIMin = 0))
	int32 AmmoToConsumePerShot = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", meta = (ClampMin = 0, UIMin = 0, ToolTip = "Index of the magazine to use from the Magazines Manager."))
	int32 MagazineIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = (ClampMin = "0", UIMin = "0"))
	float Damage = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = (ClampMin = "0", UIMin = "0", ToolTip = "Rounds per minute"))
	float FireRate = 300.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	EShootType ShootType = EShootType::Sequential;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = (ClampMin = "0", UIMin = "0", ToolTip = "Maximum effective range of the weapon in meters."))
	float MaxRange = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Modes")
	TArray<UShootModeBase*> ShootModes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	bool bHasRecoil = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil", meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float RecoilStrength = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil", meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float RecoilDuration = .1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil", meta = (EditCondition = "bHasRecoil", EditConditionHides, ToolTip = "Y (vertical) and Z (horizontal) define recoil per shot (X = shots, Y/Z = strength), X defines decay over time."))
	UCurveVector* RecoilCurve = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread")
	bool bHasSpread = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread", meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "360", UIMax = "360"))
	float MaxSpread = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread", meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "360", UIMax = "360"))
	float DefaultSpread = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread", meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float SpreadChangeRate = 15.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread", meta = (EditCondition = "bHasSpread", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float SpreadRecoveryRate = 15.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread", meta = (EditCondition = "bHasSpread", EditConditionHides, ToolTip = "Curve defines spread per shot (X = shot index, Y = spread value)."))
	UCurveFloat* SpreadCurve = nullptr;
};
