// Copyright VUEDK, Inc. All Rights Reserved.

#include "CheckpointsManager.h"
#include "Data/SaveData/CheckpointsSaveData.h"
#include "Utility/CSUtility.h"

UCheckpointsManager::UCheckpointsManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FCheckpointsSaveData UCheckpointsManager::CreateSaveData() const
{
	FCheckpointsSaveData SaveData;
	SaveData.CheckpointIndex = CheckpointIndex;
	SaveData.CurrentCheckpoints = CheckpointsMap;
	return SaveData;
}

void UCheckpointsManager::LoadSaveData(const FCheckpointsSaveData SaveData)
{
	CheckpointIndex = SaveData.CheckpointIndex;
	CheckpointsMap = SaveData.CurrentCheckpoints;
}

bool UCheckpointsManager::TrySetCheckpoint(const FName CheckpointKey, const FTransform CheckpointTransform)
{
	if (CheckpointKey == NAME_None)
	{
		UE_LOG(LogCheckpointSystem, Warning, TEXT("UCheckpointsManager::TrySetCheckpoint: Checkpoint key is not valid."));
		return false;
	}

	if (ContainsCheckpointWithKey(CheckpointKey))
	{
		UE_LOG(LogCheckpointSystem, Warning, TEXT("UCheckpointsManager::TrySetCheckpoint: Checkpoint with key %s already exists."), *CheckpointKey.ToString());
		return false;
	}
	
	const FCheckpointIndexData CheckpointIndexData = FCheckpointIndexData(CheckpointIndex++, CheckpointKey);
	CheckpointsMap.Add(CheckpointIndexData, CheckpointTransform);
	OnCheckpointSet.Broadcast();
	return true;
}

bool UCheckpointsManager::TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform)
{
	if (!ContainsCheckpointWithKey(CheckpointKey))
	{
		UE_LOG(LogCheckpointSystem, Warning, TEXT("UCheckpointsManager::TryGetMapCheckpointByKey: Checkpoint with key %s does not exist."), *CheckpointKey.ToString());
		return false;
	}
	
	OutCheckpointTransform = GetCheckpointTransformWithKey(CheckpointKey);
	return true;
}

bool UCheckpointsManager::TryGetLastCheckpoint(FTransform& OutCheckpointTransform) const
{
	if (CheckpointsMap.Num() == 0)
	{
		UE_LOG(LogCheckpointSystem, Warning, TEXT("UCheckpointsManager::TryGetLastCheckpoint: No checkpoints available."));
		return false;
	}

	int32 Index = MIN_int32;
	for (const auto& Checkpoint : CheckpointsMap)
	{
		if (Checkpoint.Key.CheckpointIndex >= Index)
		{
			Index = Checkpoint.Key.CheckpointIndex;
			OutCheckpointTransform = Checkpoint.Value;
		}
	}

	return true;
}

void UCheckpointsManager::BeginPlay()
{
	Super::BeginPlay();
	UCSUtility::Init(this);
}

bool UCheckpointsManager::ContainsCheckpointWithKey(const FName CheckpointKey)
{
	for (const auto& Checkpoint : CheckpointsMap)
	{
		if (Checkpoint.Key.CheckpointName == CheckpointKey)
			return true;
	}

	return false;
}

FTransform UCheckpointsManager::GetCheckpointTransformWithKey(const FName CheckpointKey)
{
	for (const auto& Checkpoint : CheckpointsMap)
	{
		if (Checkpoint.Key.CheckpointName == CheckpointKey)
			return Checkpoint.Value;
	}

	return FTransform();
}
