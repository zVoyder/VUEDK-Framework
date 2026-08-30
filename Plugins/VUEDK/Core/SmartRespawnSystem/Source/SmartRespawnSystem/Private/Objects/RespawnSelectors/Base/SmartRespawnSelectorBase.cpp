// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/RespawnSelectors/Base/SmartRespawnSelectorBase.h"

bool USmartRespawnSelectorBase::ChoosePlayerRespawn_Implementation(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	OutRespawnTag = FGameplayTag::EmptyTag;
	OutRespawnSnapshot = FSmartRespawnSnapshot();
	return false;
}

bool USmartRespawnSelectorBase::FindBestRespawnByPriority(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	if (Respawns.IsEmpty())
		return false;
	
	for (const auto& Pair : Respawns)
	{
		const auto& Tag = Pair.Key;
		const auto& Snapshot = Pair.Value;

		if (Snapshot.Priority >= OutRespawnSnapshot.Priority)
		{
			OutRespawnTag = Tag;
			OutRespawnSnapshot = Snapshot;
		}
	}

	return true;
}

bool USmartRespawnSelectorBase::FindBestRespawnMostRecent(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	if (Respawns.IsEmpty())
		return false;
	
	for (const auto& Pair : Respawns)
	{
		const auto& Tag = Pair.Key;
		const auto& Snapshot = Pair.Value;

		if (Snapshot.Timestamp >= OutRespawnSnapshot.Timestamp)
		{
			OutRespawnTag = Tag;
			OutRespawnSnapshot = Snapshot;
		}
	}

	return true;
}
