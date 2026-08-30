// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BattlePassRewardData.h"
#include "Engine/DataAsset.h"
#include "BattlePassData.generated.h"

USTRUCT(BlueprintType)
struct FBattlePassTierData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (ToolTip = "If true, the tier will be locked by default."))
	bool bIsLockedByDefault = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TierName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TierDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UBattlePassRewardData*> Rewards;
};

UCLASS()
class BATTLEPASSSYSTEM_API UBattlePassData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (ToolTip = "The curve that defines the experience per each reward, where the X axis is the player level and the Y axis is the experience required to reach that level."))
	UCurveFloat* ProgressionCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StartingLevel = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, FBattlePassTierData> Tiers;
};
