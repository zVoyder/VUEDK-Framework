// Copyright VUEDK, Inc. All Rights Reserved.

#include "UObjects/Tasks/TaskBase.h"

void UTaskBase::Init(UTaskData* InitData, UQuestBase* Quest)
{
	TaskData = InitData;
	RelatedQuest = Quest;
}

FTaskSaveData UTaskBase::CreateTaskSaveData() const
{
	FTaskSaveData TaskSaveData = FTaskSaveData();
	TaskSaveData.bIsAchieved = bIsAchieved;
	return TaskSaveData;
}

void UTaskBase::LoadSaveData(const FTaskSaveData TaskSaveData)
{
	bIsAchieved = TaskSaveData.bIsAchieved;
	if (bIsAchieved)
		AchieveTask(true);
}

void UTaskBase::AchieveTask(bool bFullyAchieve)
{
}

void UTaskBase::ResetTask()
{
	bIsAchieved = false;
}

void UTaskBase::TriggerAchievement()
{
	bIsAchieved = true;
	OnTaskAchieved.Broadcast();
}
