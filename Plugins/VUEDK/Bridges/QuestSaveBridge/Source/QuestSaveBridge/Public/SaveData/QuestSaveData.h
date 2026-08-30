// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskSaveData.h"
#include "Data/Enums/QuestStatus.h"
#include "Data/SaveData/SaveDataBase.h"
#include "QuestSaveData.generated.h"

UCLASS()
class QUESTSAVEBRIDGE_API UQuestSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly)
	EQuestStatus SavedStatus;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UTaskSaveData*> SavedTasks;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly)
	int32 SavedTaskIndex;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
