// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/BattlePassReward.h"
#include "Objects/BattlePassTier.h"
#include "BattlePassSystem.h"

void UBattlePassReward::Init(UBattlePassTier* BattlePassTier, UBattlePassRewardData* InRewardData)
{
	RewardData = InRewardData;
	RelatedBattlePassTier = BattlePassTier;
	
	if (!Check())
	{
		UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassReward::Init: Check failed."));
		return;
	}

	RelatedBattlePassTier->OnUnlockableStateChanged.AddDynamic(this, &UBattlePassReward::OnTierStateChanged);
}

bool UBattlePassReward::IsLocked() const
{
	return Super::IsLocked() || RelatedBattlePassTier->IsLocked();
}

UBattlePassRewardData* UBattlePassReward::GetRewardData() const
{
	return RewardData;
}

UBattlePassTier* UBattlePassReward::GetRelatedTier() const
{
	return RelatedBattlePassTier;
}

void UBattlePassReward::OnTierStateChanged(const bool bLockedState)
{
	NotifyState();
}

bool UBattlePassReward::Check() const
{
	return IsValid(RelatedBattlePassTier) && IsValid(RewardData);
}
