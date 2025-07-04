// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskBase.h"
#include "UObjects/Quests/QuestBase.h"
#include "CountTask.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class QUESTSYSTEM_API UCountTask : public UTaskBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	int32 CountToAchieve;
	
private:
	int32 CurrentCount;

public:
	virtual void Init(UTaskData* InitData, UQuestBase* Quest) override;
	
	virtual void LoadSaveData(FTaskSaveData TaskSaveData) override;
	
	virtual void AchieveTask(const bool bFullyAchieve = false) override;
	
	virtual FTaskSaveData CreateTaskSaveData() const override;
	
	virtual void ResetTask() override;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentCount() const;
};
