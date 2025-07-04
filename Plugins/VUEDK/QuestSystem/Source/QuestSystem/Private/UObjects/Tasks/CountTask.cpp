// Copyright VUEDK, Inc. All Rights Reserved.

#include "UObjects/Tasks/CountTask.h"
#include "Data/DataAssets/CountTaskData.h"

void UCountTask::Init(UTaskData* InitData, UQuestBase* Quest)
{
	Super::Init(InitData, Quest);

	CurrentCount = 0;
	if (const UCountTaskData* CountTaskData = Cast<UCountTaskData>(InitData))
		CountToAchieve = CountTaskData->CountToAchieve;
}

void UCountTask::LoadSaveData(FTaskSaveData TaskSaveData)
{
	Super::LoadSaveData(TaskSaveData);
	CurrentCount = TaskSaveData.CurrentAchieveCount;
}

void UCountTask::AchieveTask(const bool bFullyAchieve)
{
	if (bIsAchieved) return;

	if (bFullyAchieve)
	{
		CurrentCount = CountToAchieve;
		TriggerAchievement();
	}
	else
	{
		CurrentCount++;
		if (CurrentCount >= CountToAchieve)
			TriggerAchievement();
	}
}

FTaskSaveData UCountTask::CreateTaskSaveData() const
{
	FTaskSaveData TaskSaveData = FTaskSaveData();
	TaskSaveData.bIsAchieved = bIsAchieved;
	TaskSaveData.CurrentAchieveCount = CurrentCount;
	return TaskSaveData;
}

void UCountTask::ResetTask()
{
	Super::ResetTask();
	CurrentCount = 0;
}

int32 UCountTask::GetCurrentCount() const
{
	return CurrentCount;
}
