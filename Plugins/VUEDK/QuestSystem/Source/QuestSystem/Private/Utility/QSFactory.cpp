// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/QSFactory.h"
#include "Data/Structs/QuestEntryData.h"
#include "UObjects/Quests/QuestParallel.h"
#include "UObjects/Quests/QuestSequencial.h"
#include "UObjects/Tasks/CountTask.h"
#include "UObjects/Tasks/SingleTask.h"

UQuestBase* UQSFactory::CreateQuestByType(UQuestData* QuestData, const FQuestEntryData& QuestEntryData)
{
	UQuestParallel* QuestParallel = nullptr;
	UQuestSequencial* QuestSequencial = nullptr;
	
	switch (QuestEntryData.QuestType)
	{
		case EQuestType::None:
			return nullptr;
		
		case EQuestType::Parallel:
			QuestParallel = NewObject<UQuestParallel>();
			QuestParallel->Init(QuestData, QuestEntryData);
			return QuestParallel;
		
		case EQuestType::Sequential:
			QuestSequencial = NewObject<UQuestSequencial>();
			QuestSequencial->Init(QuestData, QuestEntryData);
			return QuestSequencial;
	}
	
	return nullptr;
}

UTaskBase* UQSFactory::CreateTaskByType(UQuestBase* RelatedQuest, UTaskData* TaskData, const ETaskType TaskType)
{
	USingleTask* SingleTask = nullptr;
	UCountTask* CountTask = nullptr;

	switch (TaskType)
	{
		case ETaskType::None:
			return nullptr;

		case ETaskType::Single:
			SingleTask = NewObject<USingleTask>();
			SingleTask->Init(TaskData, RelatedQuest);
			return SingleTask;

		case ETaskType::Count:
			CountTask = NewObject<UCountTask>();
			CountTask->Init(TaskData, RelatedQuest);
			return CountTask;
	}

	return nullptr;
}
