// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CheckpointIndexData.h"
#include "Data/SaveData/SaveDataBase.h"
#include "CheckpointsSaveData.generated.h"

struct FCheckpointIndexData;

UCLASS()
class CHECKPOINTSAVEBRIDGE_API UCheckpointsSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	TMap<FCheckpointIndexData, FTransform> SavedCheckpoints;
	UPROPERTY(SaveGame)
	int32 SavedCheckpointIndex = 0;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
