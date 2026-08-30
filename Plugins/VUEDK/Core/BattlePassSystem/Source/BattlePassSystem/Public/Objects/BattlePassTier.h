// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BattlePassReward.h"
#include "Base/UnlockableBase.h"
#include "Data/BattlePassData.h"
#include "UObject/Object.h"
#include "BattlePassTier.generated.h"

class UBattlePass;

UCLASS(NotBlueprintable, BlueprintType)
class BATTLEPASSSYSTEM_API UBattlePassTier : public UUnlockableBase
{
	GENERATED_BODY()

private:
	int32 TierIndex;
	FBattlePassTierData BattlePassTierData;
	UPROPERTY()
	UBattlePass* RelatedBattlePass;
	UPROPERTY()
	TMap<UBattlePassRewardData*, UBattlePassReward*> Rewards;

public:
	void Init(UBattlePass* BattlePass, const int32 InTierIndex, const FBattlePassTierData& InBattlePassTierData);

	UFUNCTION(BlueprintCallable)
	virtual void Unlock() override;

	UFUNCTION(BlueprintCallable)
	virtual void Lock() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetLockedState(const bool bLocked, const bool bNotify = true) override;
	
	UFUNCTION(BlueprintPure)
	const FBattlePassTierData& GetBattlePassTierData() const;

	UFUNCTION(BlueprintPure)
	UBattlePassReward* FindReward(const UBattlePassRewardData* InRewardData) const;

	UFUNCTION(BlueprintPure)
	TArray<UBattlePassReward*> FindRewardsByTag(const FGameplayTag InTag) const;

	UFUNCTION(BlueprintPure)
	const TMap<UBattlePassRewardData*, UBattlePassReward*>& GetRewards() const;

	UFUNCTION(BlueprintPure)
	UBattlePass* GetRelatedBattlePass() const;

	UFUNCTION(BlueprintPure)
	int32 GetTierIndex() const;

protected:
	virtual bool CanChangeState() const override;

private:
	void CreateRewards();

	bool Check() const;
};
