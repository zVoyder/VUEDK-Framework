#pragma once

#include "CoreMinimal.h"
#include "CheckpointsSaveData.generated.h"

USTRUCT(BlueprintType)
struct CHECKPOINTSYSTEM_API FCheckpointsSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FCheckpointIndexData, FTransform> CurrentCheckpoints;
	UPROPERTY()
	int32 CheckpointIndex = 0;
};
