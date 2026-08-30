// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/UnlockableBase.h"
#include "Data/BattlePassData.h"
#include "UObject/Object.h"
#include "BattlePassReward.generated.h"

class UBattlePassTier;

UCLASS(NotBlueprintable, BlueprintType)
class BATTLEPASSSYSTEM_API UBattlePassReward : public UUnlockableBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBattlePassRewardData* RewardData;
	UPROPERTY()
	UBattlePassTier* RelatedBattlePassTier;

public:
	void Init(UBattlePassTier* BattlePassTier, UBattlePassRewardData* InRewardData);

	virtual bool IsLocked() const override;
	
	UFUNCTION(BlueprintPure)
	UBattlePassRewardData* GetRewardData() const;

	UFUNCTION(BlueprintPure)
	UBattlePassTier* GetRelatedTier() const;

private:
	UFUNCTION()
	void OnTierStateChanged(const bool bLockedState);
	
	bool Check() const;
};
