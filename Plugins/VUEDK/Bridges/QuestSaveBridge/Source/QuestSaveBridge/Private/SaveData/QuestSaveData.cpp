// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/QuestSaveData.h"
#include "UObjects/Quests/QuestBase.h"
#include "UObjects/Quests/QuestSequencial.h"
#include "UObjects/Tasks/CountTask.h"
#include "UObjects/Tasks/TaskBase.h"

bool UQuestSaveData::SaveObjectData(UObject* ObjectToSave)
{
	UQuestBase* Quest = Cast<UQuestBase>(ObjectToSave);
	
	if (!IsValid(Quest))
		return false;

	SavedStatus = Quest->GetQuestStatus();
	
	for (const auto& Task : Quest->AllTasks)
	{
		UTaskSaveData* TaskSaveData = NewObject<UTaskSaveData>();
		UTaskBase* TaskBase = Task.Value;
		TaskSaveData->SaveObjectData(TaskBase);
		SavedTasks.Add(Task.Key->GetFName(), TaskSaveData);
	}

	const UQuestSequencial* SequencialQuest = Cast<UQuestSequencial>(Quest);
	if (IsValid(SequencialQuest))
		return false;
	
	SavedTaskIndex = SequencialQuest->GetCurrentTaskIndex();
	return Super::SaveObjectData(ObjectToSave);
}

bool UQuestSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	UQuestBase* Quest = Cast<UQuestBase>(ObjectToLoad);
	
	if (!IsValid(Quest))
		return false;
	
	Quest->ResetQuest();
	for (const auto TaskSaveData : SavedTasks)
	{
		UTaskBase* Task = Quest->GetTaskByFName(TaskSaveData.Key);
		if (!Task)
			continue;
		
		TaskSaveData.Value->LoadObjectData(Task);
	}
	
	return Super::LoadObjectData(ObjectToLoad);
}
