// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Stats/RPGGearItemStatsContainer.h"

float URPGGearItemStatsContainer::ValidateStatValue(const float Value, const FFloatRange& Range)
{
	// No need to clamp value between min and max, just return the value
	return Value;
}
