// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveModes/Base/BattlePassSaveModeBase.h"

void UBattlePassSaveModeBase::Init(UBattlePassSubsystem* InSubSystem)
{
	BattlePassSubsystem = InSubSystem;
}

void UBattlePassSaveModeBase::Save()
{
}

void UBattlePassSaveModeBase::Load()
{
}

FBattlePassesSaveData UBattlePassSaveModeBase::CreateSaveData() const
{
	const TMap<FGameplayTag, UBattlePass*>& AllBattlePasses = BattlePassSubsystem->GetAllBattlePasses();
	return FBattlePassesSaveData(AllBattlePasses);
}

void UBattlePassSaveModeBase::CompleteLoad(const FBattlePassesSaveData& LoadedData) const
{
	OnBattlePassLoadComplete.ExecuteIfBound(LoadedData);
}

void UBattlePassSaveModeBase::FailLoad(const bool bIsError) const
{
	OnBattlePassLoadFailed.ExecuteIfBound(bIsError);
}
