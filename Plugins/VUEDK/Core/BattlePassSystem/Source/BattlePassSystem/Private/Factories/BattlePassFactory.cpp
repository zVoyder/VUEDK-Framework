// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/BattlePassFactory.h"

UBattlePassProgression* UBattlePassFactory::CreateBattlePassProgression(UBattlePass* RelatedBattlePass)
{
	if (!IsValid(RelatedBattlePass))
		return nullptr;

	UBattlePassProgression* NewBattlePassProgression = NewObject<UBattlePassProgression>(GetTransientPackage());
	if (!IsValid(NewBattlePassProgression))
		return nullptr;

	NewBattlePassProgression->Init(RelatedBattlePass);
	return NewBattlePassProgression;
}

UBattlePass* UBattlePassFactory::CreateBattlePass(const FGameplayTag& BattlePassTag, UBattlePassData* BattlePassData, const bool bIsActive)
{
	if (!IsValid(BattlePassData))
		return nullptr;

	UBattlePass* NewBattlePass = NewObject<UBattlePass>(GetTransientPackage());
	if (!IsValid(NewBattlePass))
		return nullptr;

	NewBattlePass->Init(BattlePassTag, BattlePassData, bIsActive);
	return NewBattlePass;
}

UBattlePassTier* UBattlePassFactory::CreateBattlePassTier(UBattlePass* BattlePass, const int32 TierIndex, const FBattlePassTierData& BattlePassTierData)
{
	UBattlePassTier* NewBattlePassTier = NewObject<UBattlePassTier>(GetTransientPackage());
	if (!IsValid(NewBattlePassTier))
		return nullptr;

	NewBattlePassTier->Init(BattlePass, TierIndex, BattlePassTierData);
	return NewBattlePassTier;
}

UBattlePassReward* UBattlePassFactory::CreateBattlePassReward(UBattlePassTier* BattlePassTier, UBattlePassRewardData* BattlePassRewardData)
{
	if (!IsValid(BattlePassRewardData))
		return nullptr;

	UBattlePassReward* NewBattlePassReward = NewObject<UBattlePassReward>(GetTransientPackage());
	if (!IsValid(NewBattlePassReward))
		return nullptr;

	NewBattlePassReward->Init(BattlePassTier, BattlePassRewardData);
	return NewBattlePassReward;
}

UBattlePassSaveModeBase* UBattlePassFactory::CreateBattlePassSaveMode(const TSoftClassPtr<UBattlePassSaveModeBase> SaveModeClass, UBattlePassSubsystem* Subsystem)
{
	if (!SaveModeClass.IsValid())
		return nullptr;

	const TSubclassOf<UBattlePassSaveModeBase> LoadedClass = SaveModeClass.LoadSynchronous();
	if (!IsValid(LoadedClass))
		return nullptr;

	UBattlePassSaveModeBase* NewSaveMode = NewObject<UBattlePassSaveModeBase>(GetTransientPackage(), LoadedClass);
	if (!IsValid(NewSaveMode))
		return nullptr;

	NewSaveMode->Init(Subsystem);
	return NewSaveMode;
}
