// Copyright VUEDK, Inc. All Rights Reserved.

#include "UObjects/Quests/QuestSequencial.h"
#include "UObjects/Tasks/TaskBase.h"

void UQuestSequencial::Init(UQuestData* InitData, const FQuestEntryData& EntryData)
{
	Super::Init(InitData, EntryData);
	TasksByIndex = InitData->TasksData;
}

void UQuestSequencial::LoadSaveData(FQuestSaveData QuestSaveData)
{
	Super::LoadSaveData(QuestSaveData);
	CurrentTaskIndex = QuestSaveData.CurrentTaskIndex;
}

void UQuestSequencial::AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve)
{
	if (bIsQuestCompleted) return;
	UTaskBase* Task = GetTask(TaskDataKey);
	if (!Task) return;
	
	if (bFullyAchieve)
	{
		Task->AchieveTask(true);
		bIsQuestCompleted = true;
		return;
	}
	
	if (CurrentTaskIndex > TasksByIndex.Num() - 1 || CurrentTaskIndex < 0) return;
	
	if (TasksByIndex[CurrentTaskIndex] == TaskDataKey) // Checks if the task is at the current index
	{
		Task->AchieveTask();
		if (Task->bIsAchieved) // Because the task may not be achieved depending on the task logic
			CurrentTaskIndex++;

		bIsQuestCompleted = AreAllTasksAchieved();
	}
}

void UQuestSequencial::ResetQuest()
{
	Super::ResetQuest();
	CurrentTaskIndex = 0;
}

FQuestSaveData UQuestSequencial::CreateQuestSaveData() const
{
	FQuestSaveData QuestSaveData = Super::CreateQuestSaveData();
	QuestSaveData.CurrentTaskIndex = CurrentTaskIndex;
	return QuestSaveData;
}

int32 UQuestSequencial::GetCurrentTaskIndex() const
{
	return CurrentTaskIndex;
}

int32 UQuestSequencial::GetTaskIndex(UTaskData* TaskDataKey) const
{
	const UTaskBase* Task = GetTask(TaskDataKey);
	if (!Task) return -1;

	return TasksByIndex.Find(TaskDataKey);
}
