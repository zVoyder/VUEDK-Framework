// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CheckpointIndexData.generated.h"

USTRUCT()
struct CHECKPOINTSYSTEM_API FCheckpointIndexData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	int32 CheckpointIndex;
	UPROPERTY(SaveGame)
	FName CheckpointName;

	FCheckpointIndexData()
		: CheckpointIndex(0),
		  CheckpointName(NAME_None)
	{
	}

	FCheckpointIndexData(const int32 InCheckpointIndex, const FName InCheckpointName): CheckpointIndex(InCheckpointIndex),
	                                                                       CheckpointName(InCheckpointName)
	{
	}
};

FORCEINLINE uint32 GetTypeHash(const FCheckpointIndexData& CheckpointIndex)
{
	const uint32 Hash = FCrc::MemCrc32(&CheckpointIndex, sizeof(FCheckpointIndexData));
	return Hash;
}

FORCEINLINE bool operator==(const FCheckpointIndexData& A, const FCheckpointIndexData& B)
{
	return A.CheckpointIndex == B.CheckpointIndex && A.CheckpointName == B.CheckpointName;
}
