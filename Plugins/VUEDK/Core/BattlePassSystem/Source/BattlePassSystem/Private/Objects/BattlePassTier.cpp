// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/BattlePassTier.h"
#include "Objects/BattlePass.h"
#include "Factories/BattlePassFactory.h"
#include "BattlePassSystem.h"

void UBattlePassTier::Init(UBattlePass* BattlePass, const int32 InTierIndex, const FBattlePassTierData& InBattlePassTierData)
{
	RelatedBattlePass = BattlePass;
	BattlePassTierData = InBattlePassTierData;
	TierIndex = InTierIndex;
	
	if (!Check())
	{
		UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassTier::Init: Check failed."));
		return;
	}

	SetLockedState(BattlePassTierData.bIsLockedByDefault, false);
	CreateRewards();
}

void UBattlePassTier::Unlock()
{
	Super::Unlock();
}

void UBattlePassTier::Lock()
{
	Super::Lock();
}

void UBattlePassTier::SetLockedState(const bool bLocked, const bool bNotify)
{
	Super::SetLockedState(bLocked, bNotify);
}

const FBattlePassTierData& UBattlePassTier::GetBattlePassTierData() const
{
	return BattlePassTierData;
}

UBattlePassReward* UBattlePassTier::FindReward(const UBattlePassRewardData* InRewardData) const
{
	if (!IsValid(InRewardData))
		return nullptr;

	return Rewards.Contains(InRewardData) ? Rewards[InRewardData] : nullptr;
}

TArray<UBattlePassReward*> UBattlePassTier::FindRewardsByTag(const FGameplayTag InTag) const
{
	TArray<UBattlePassReward*> RewardsWithTag;
	for (const TPair<UBattlePassRewardData*, UBattlePassReward*>& RewardPair : Rewards)
	{
		if (RewardPair.Key->Tags.Contains(InTag))
			RewardsWithTag.Add(RewardPair.Value);
	}
	
	return RewardsWithTag;
}

const TMap<UBattlePassRewardData*, UBattlePassReward*>& UBattlePassTier::GetRewards() const
{
	return Rewards;
}

UBattlePass* UBattlePassTier::GetRelatedBattlePass() const
{
	return RelatedBattlePass;
}

int32 UBattlePassTier::GetTierIndex() const
{
	return TierIndex;
}

bool UBattlePassTier::CanChangeState() const
{
	return Check() && RelatedBattlePass->IsActive();
}

void UBattlePassTier::CreateRewards()
{
	for (UBattlePassRewardData* RewardData : BattlePassTierData.Rewards)
	{
		if (!IsValid(RewardData))
			continue;

		UBattlePassReward* NewReward = UBattlePassFactory::CreateBattlePassReward(this, RewardData);
		if (!IsValid(NewReward))
		{
			UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassTier::CreateRewardss: Failed to create Battle Pass Reward for reward data %s."), *GetNameSafe(RewardData));
			continue;
		}

		Rewards.Add(RewardData, NewReward);
	}
}

bool UBattlePassTier::Check() const
{
	return IsValid(RelatedBattlePass);
}
