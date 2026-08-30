// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsSystem/Containers/CoreStatsContainer.h"
#include "RPGGearItemStatsContainer.generated.h"

UCLASS()
class RPGSYSTEM_API URPGGearItemStatsContainer : public UCoreStatsContainer
{
	GENERATED_BODY()

protected:
	virtual float ValidateStatValue(const float Value, const FFloatRange& Range) override;
};
