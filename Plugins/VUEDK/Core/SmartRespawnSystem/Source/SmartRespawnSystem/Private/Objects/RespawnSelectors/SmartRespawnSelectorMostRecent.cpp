// Copyright VU, Inc. All Rights Reserved.

#include "Objects/RespawnSelectors/SmartRespawnSelectorMostRecent.h"

bool USmartRespawnSelectorMostRecent::ChoosePlayerRespawn_Implementation(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	return FindBestRespawnMostRecent(Respawns, OutRespawnTag, OutRespawnSnapshot);
}
