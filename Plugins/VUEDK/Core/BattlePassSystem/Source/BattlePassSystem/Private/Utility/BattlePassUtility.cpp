// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/BattlePassUtility.h"
#include "JsonObjectConverter.h"

TWeakObjectPtr<UBattlePassSubsystem> UBattlePassUtility::BattlePassSubsystem = nullptr;

void UBattlePassUtility::Init(UBattlePassSubsystem* InBattlePassSubsystem)
{
	UBattlePassUtility::BattlePassSubsystem = InBattlePassSubsystem;
}

UBattlePass* UBattlePassUtility::GetBattlePass(const FGameplayTag BattlePassTag)
{
	if (!BattlePassSubsystem.IsValid())
		return nullptr;

	return BattlePassSubsystem.Get()->GetBattlePass(BattlePassTag);
}
