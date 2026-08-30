// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/VGOAPUtility.h"

TWeakObjectPtr<UGOAPBlackboardsSubsystem> UVGOAPUtility::BlackboardsSubsystem = nullptr;

void UVGOAPUtility::Init(UGOAPBlackboardsSubsystem* InBlackboardsSubsystem)
{
	BlackboardsSubsystem = InBlackboardsSubsystem;
}

UGOAPBlackboard* UVGOAPUtility::GetGlobalBlackboard(const FGameplayTag BlackboardTag)
{
	if (!BlackboardsSubsystem.IsValid())
		return nullptr;
	
	return BlackboardsSubsystem.Get()->GetGlobalBlackboard(BlackboardTag);
}
