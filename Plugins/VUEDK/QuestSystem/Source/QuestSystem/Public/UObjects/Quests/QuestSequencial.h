// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestBase.h"
#include "QuestSequencial.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class QUESTSYSTEM_API UQuestSequencial : public UQuestBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UTaskData*> TasksByIndex;

private:
	int32 CurrentTaskIndex = 0;
	
public:
	virtual void Init(UQuestData* InitData, const FQuestEntryData& EntryData) override;
	
	virtual void LoadSaveData(FQuestSaveData QuestSaveData) override;
	
	virtual void AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve) override;
	
	virtual void ResetQuest() override;
	
	virtual FQuestSaveData CreateQuestSaveData() const override;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentTaskIndex() const;

	UFUNCTION(BlueprintPure)
	int32 GetTaskIndex(UTaskData* TaskDataKey) const;
};
