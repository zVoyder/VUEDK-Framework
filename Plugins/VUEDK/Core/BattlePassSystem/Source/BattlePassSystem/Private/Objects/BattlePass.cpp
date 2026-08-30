// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/BattlePass.h"
#include "Factories/BattlePassFactory.h"
#include "BattlePassSystem.h"
#include "Data/BattlePassesSaveData.h"
#include "Objects/BattlePassProgression.h"

void UBattlePass::Init(const FGameplayTag& InBattlePassTag, UBattlePassData* InBattlePassData, const bool bInIsActive)
{
	BattlePassTag = InBattlePassTag;
	BattlePassData = InBattlePassData;
	bIsActive = bInIsActive;
	CreateProgression();

	if (!Check())
	{
		UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePass::Init: Check failed."));
		return;
	}

	CreateTiers();
}

void UBattlePass::LoadBattlePass(const FBattlePassSaveData& BattlePassSaveData)
{
	for (const auto& TierPair : BattlePassSaveData.SavedTiers)
	{
		const int32 TierIndex = TierPair.Key;
		const bool bIsLocked = TierPair.Value;

		UBattlePassTier* Tier = FindTier(TierIndex);
		if (!IsValid(Tier))
			continue;

		Tier->SetLockedState(bIsLocked, false); // Set the tier's locked state without notifying to avoid unnecessary effects during loading
	}
	
	Progression->LoadProgression(BattlePassSaveData);
	UpdateRewards(false);
}

UBattlePassData* UBattlePass::GetBattlePassData() const
{
	return BattlePassData;
}

UBattlePassTier* UBattlePass::FindTier(const int32 InTier) const
{
	return Tiers.FindRef(InTier);
}

TMap<int32, UBattlePassTier*> UBattlePass::GetTiers() const
{
	return Tiers;
}

bool UBattlePass::IsActive() const
{
	return Check() && bIsActive;
}

void UBattlePass::UnlockTier(const int32 InTier) const
{
	if (!IsActive())
		return;

	if (Tiers.Contains(InTier))
		Tiers[InTier]->Unlock();
}

void UBattlePass::LockTier(const int32 InTier) const
{
	if (!IsActive())
		return;

	if (Tiers.Contains(InTier))
		Tiers[InTier]->Lock();
}

void UBattlePass::AddLevelExperience(const float Experience) const
{
	if (!IsActive())
		return;

	Progression->AddLevelExperience(Experience);
}

void UBattlePass::RemoveLevelExperience(const float Experience) const
{
	if (!IsActive())
		return;

	Progression->RemoveLevelExperience(Experience);
}

float UBattlePass::GetLevelExperience() const
{
	if (!Check())
		return 0.0f;

	return Progression->GetLevelExperience();
}

float UBattlePass::GetTotalExperience() const
{
	if (!Check())
		return 0.0f;

	return Progression->GetTotalExperience();
}

float UBattlePass::GetExperienceForNextLevel() const
{
	if (!Check())
		return 0.0f;

	return Progression->GetExperienceForNextLevel();
}

float UBattlePass::GetExperienceForLevel(const int32 Level) const
{
	if (!Check())
		return 0.0f;

	return Progression->GetExperienceForLevel(Level);
}

int32 UBattlePass::GetLevel() const
{
	if (!Check())
		return 0;

	return Progression->GetLevel();
}

UBattlePassProgression* UBattlePass::GetProgression() const
{
	return Progression;
}

FGameplayTag UBattlePass::GetBattlePassTag() const
{
	return BattlePassTag;
}

void UBattlePass::CreateProgression()
{
	Progression = UBattlePassFactory::CreateBattlePassProgression(this);

	if (!IsValid(Progression))
	{
		UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePass::CreateProgression: Failed to create Battle Pass Progression."));
		return;
	}

	Progression->OnLevelChanged.AddUniqueDynamic(this, &UBattlePass::OnLevelChanged);
}

void UBattlePass::CreateTiers()
{
	for (const auto& Tier : BattlePassData->Tiers)
	{
		UBattlePassTier* NewTier = UBattlePassFactory::CreateBattlePassTier(this, Tier.Key, Tier.Value);
		if (!IsValid(NewTier))
		{
			UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePass::CreateTiers: Failed to create Battle Pass Tier for tier %d."), Tier.Key);
			continue;
		}

		Tiers.Add(Tier.Key, NewTier);
		NewTier->OnUnlockableStateChanged.AddUniqueDynamic(this, &UBattlePass::OnTierStateChanged);
	}
}

void UBattlePass::UpdateRewards(const bool bNotify)
{
	const int32 CurrentLevel = Progression->GetLevel();

	for (const auto& Pair : Tiers)
	{
		int32 TierIndex = Pair.Key;
		const UBattlePassTier* Tier = Pair.Value;

		TMap<UBattlePassRewardData*, UBattlePassReward*> Rewards = Tier->GetRewards();
		for (const auto& RewardPair : Rewards)
		{
			const UBattlePassRewardData* RewardData = RewardPair.Key;
			UBattlePassReward* Reward = RewardPair.Value;

			if (!IsValid(Reward) || !IsValid(RewardData))
				continue;

			const bool bIsLocked = CurrentLevel < RewardData->UnlockLevel;
			Reward->SetLockedState(bIsLocked, bNotify); // The reward considers the tier's locked state.
		}
	}

	if (bNotify)
		OnUpdateRewards.ExecuteIfBound();
}

void UBattlePass::OnLevelChanged(int32 NewLevel, int32 OldLevel)
{
	UpdateRewards();
}

void UBattlePass::OnTierStateChanged(const bool bIsLocked)
{
	UpdateRewards();
}

bool UBattlePass::Check() const
{
	return IsValid(BattlePassData) && IsValid(Progression);
}
