// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutoSaveManagerData.generated.h"

USTRUCT(BlueprintType)
struct FAutoSaveManagerData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bInitPauseState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "30.0"))
	float AutoSaveIntervalSeconds = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 MaxAutoSaves = 3;

	FAutoSaveManagerData(): bInitPauseState(false),
	                        AutoSaveIntervalSeconds(60.0f),
	                        MaxAutoSaves(3)
	{
	}

	FAutoSaveManagerData(const bool bInitPause, const float IntervalSeconds, const int32 MaxAutoSavesNumber): bInitPauseState(bInitPause),
	                                                                                                          AutoSaveIntervalSeconds(IntervalSeconds),
	                                                                                                          MaxAutoSaves(MaxAutoSavesNumber)
	{
	}
};
