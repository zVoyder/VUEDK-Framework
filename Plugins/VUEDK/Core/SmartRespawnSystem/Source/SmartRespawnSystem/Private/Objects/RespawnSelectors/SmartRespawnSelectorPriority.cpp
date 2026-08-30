// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/RespawnSelectors/SmartRespawnSelectorPriority.h"

bool USmartRespawnSelectorPriority::ChoosePlayerRespawn_Implementation(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	return FindBestRespawnByPriority(Respawns, OutRespawnTag, OutRespawnSnapshot);
}
