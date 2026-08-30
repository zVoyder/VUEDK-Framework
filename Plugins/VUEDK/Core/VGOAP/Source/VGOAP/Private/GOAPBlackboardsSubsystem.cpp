// Copyright VUEDK, Inc. All Rights Reserved.

#include "GOAPBlackboardsSubsystem.h"
#include "VGOAPSettings.h"
#include "Factories/GOAPFactory.h"
#include "Utility/VGOAPUtility.h"

void UGOAPBlackboardsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CreateBlackboards();
	UVGOAPUtility::Init(this);
}

UGOAPBlackboard* UGOAPBlackboardsSubsystem::GetGlobalBlackboard(const FGameplayTag& BlackboardTag) const
{
	return GlobalBlackboards.FindRef(BlackboardTag);
}

void UGOAPBlackboardsSubsystem::CreateBlackboards()
{
	GlobalBlackboards.Empty();
	const UVGOAPSettings* GOAPSettings = GetDefault<UVGOAPSettings>();
	if (!IsValid(GOAPSettings))
		return;

	for (const auto& BlackboardEntry : GOAPSettings->GlobalBlackboards)
	{
		const FGameplayTag& BlackboardTag = BlackboardEntry.Key;
		const TSoftObjectPtr<UGOAPBlackboardData>& BlackboardDataPtr = BlackboardEntry.Value;

		FSoftObjectPath AssetPath = BlackboardDataPtr.ToSoftObjectPath();

		if (!AssetPath.IsValid())
			continue;

		UGOAPBlackboardData* BlackboardData = Cast<UGOAPBlackboardData>(
			StaticLoadObject(UGOAPBlackboardData::StaticClass(), nullptr, *AssetPath.ToString())
		);

		if (!IsValid(BlackboardData))
			continue;

		UGOAPBlackboard* NewBlackboard = UGOAPFactory::CreateBlackboard(BlackboardData);
		GlobalBlackboards.Add(BlackboardTag, NewBlackboard);
	}
}
