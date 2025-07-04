// Copyright VUEDK, Inc. All Rights Reserved.

#include "UObjects/Quests/QuestBase.h"
#include "UObjects/Tasks/TaskBase.h"
#include "Utility/QSFactory.h"

void UQuestBase::Init(UQuestData* InitData, const FQuestEntryData& EntryData)
{
	QuestData = InitData;
	QuestType = EntryData.QuestType;
	QuestEntryData = EntryData;
	
	for (UTaskData* TaskData : InitData->TasksData)
	{
		UTaskBase* Task = UQSFactory::CreateTaskByType(this, TaskData, TaskData->GetTaskType());
		AllTasks.Add(TaskData, Task);
	}
}

FQuestSaveData UQuestBase::CreateQuestSaveData() const
{
	FQuestSaveData QuestSaveData = FQuestSaveData();
	
	for (const auto& Task : AllTasks)
	{
		QuestSaveData.QuestStatus = QuestStatus;
		QuestSaveData.Tasks.Add(Task.Key->GetFName(), Task.Value->CreateTaskSaveData());
	}

	return QuestSaveData;
}

void UQuestBase::LoadSaveData(FQuestSaveData QuestSaveData)
{
	ResetQuest();
	for (const auto TaskSaveData : QuestSaveData.Tasks)
	{
		UTaskBase* Task = GetTaskByFName(TaskSaveData.Key);
		if (!Task) continue; // If the task is not found, skip it
		
		Task->LoadSaveData(TaskSaveData.Value);
	}
}

void UQuestBase::AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve)
{
}

void UQuestBase::AchieveAllTasks(const bool bFullyAchieve) const
{
	if (bIsQuestCompleted) return;

	for(const TTuple<UTaskData*, UTaskBase*> Task : AllTasks)
		Task.Value->AchieveTask(bFullyAchieve);
}

bool UQuestBase::IsTaskAchieved(const UTaskData* TaskDataKey) const
{
	const UTaskBase* Task = GetTask(TaskDataKey);
	if (!Task) return false;
	
	return Task->bIsAchieved;
}

UTaskBase* UQuestBase::GetTask(const UTaskData* TaskDataKey) const
{
	return AllTasks.FindRef(TaskDataKey);
}

UTaskBase* UQuestBase::GetTaskByFName(const FName TaskName) const
{
	for (const auto& Task : AllTasks)
	{
		if (Task.Value->TaskData->GetFName() == TaskName)
			return Task.Value;
	}
	
	return nullptr;
}

void UQuestBase::SetQuestStatus(const EQuestStatus Status)
{
	QuestStatus = Status;
}

EQuestStatus UQuestBase::GetQuestStatus() const
{
	return QuestStatus;
}

EQuestType UQuestBase::GetQuestType() const
{
	return QuestType;
}

void UQuestBase::ResetQuest()
{
	SetQuestStatus(QuestEntryData.InitialQuestStatus);
	for(const auto& Task : AllTasks)
		Task.Value->ResetTask();

	bIsQuestCompleted = false;
}

bool UQuestBase::AreAllTasksAchieved() const
{
	for (auto& Task : AllTasks)
	{
		if (!Task.Value->bIsAchieved)
			return false;
	}
	return true;
}
