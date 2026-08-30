// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/SaveDataBase.h"
#include "TaskSaveData.generated.h"

UCLASS()
class QUESTSAVEBRIDGE_API UTaskSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bSavedIsAchieved;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 SavedAchieveCount;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
