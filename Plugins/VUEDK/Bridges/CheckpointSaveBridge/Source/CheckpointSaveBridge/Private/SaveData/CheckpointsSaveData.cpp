// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/CheckpointsSaveData.h"
#include "CheckpointsManager.h"

bool UCheckpointsSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const UCheckpointsManager* CheckpointsManager = Cast<UCheckpointsManager>(ObjectToSave);

	
	if (!IsValid(CheckpointsManager))
		return false;

	SavedCheckpointIndex = CheckpointsManager->GetCheckpointIndex();
	SavedCheckpoints = CheckpointsManager->GetCheckpointsMap();
	
	return Super::SaveObjectData(ObjectToSave);
}

bool UCheckpointsSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	UCheckpointsManager* CheckpointsManager = Cast<UCheckpointsManager>(ObjectToLoad);

	if (!IsValid(CheckpointsManager))
		return false;
	
	CheckpointsManager->SetCheckpointIndex(SavedCheckpointIndex);
	CheckpointsManager->SetCheckpointsMap(SavedCheckpoints);
	return Super::LoadObjectData(ObjectToLoad);
}
