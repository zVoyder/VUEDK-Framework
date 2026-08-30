// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/CSUtility.h"
#include "CheckpointsManager.h"

UCheckpointsManager* UCSUtility::CurrCheckpointsManager = nullptr;

void UCSUtility::Init(UCheckpointsManager* CheckpointsManager)
{
	CurrCheckpointsManager = CheckpointsManager;
}

bool UCSUtility::TrySetCheckpoint(const FName CheckpointKey, const FTransform& CheckpointTransform)
{
	if (!Check())
		return false;

	CurrCheckpointsManager->TrySetCheckpoint(CheckpointKey, CheckpointTransform);
	return true;
}

bool UCSUtility::TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform)
{
	if (!Check())
		return false;

	return CurrCheckpointsManager->TryGetCheckpoint(CheckpointKey, OutCheckpointTransform);
}

bool UCSUtility::TryGetLastCheckpoint(FTransform& OutCheckpointTransform, FName& OutCheckpointKey)
{
	if (!Check())
		return false;

	return CurrCheckpointsManager->TryGetLastCheckpoint(OutCheckpointTransform, OutCheckpointKey);
}

bool UCSUtility::Check()
{
	if (!CurrCheckpointsManager)
	{
		UE_LOG(LogCheckpointSystem, Error, TEXT("CheckpointsManager is null."));
		return false;
	}

	return true;
}
