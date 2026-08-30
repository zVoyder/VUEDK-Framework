// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveData.h"
#include "Data/SaveData/SaveDataBase.h"
#include "QuestLogSaveData.generated.h"

UCLASS()
class QUESTSAVEBRIDGE_API UQuestLogSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FName SavedTrackedQuest;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FName, UQuestSaveData*> SavedQuests;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
