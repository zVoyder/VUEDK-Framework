// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/QSUtility.h"
#include "QuestManager.h"

UQuestManager* UQSUtility::CurrentQuestManager = nullptr;

void UQSUtility::Init(UQuestManager* QuestManager)
{
	CurrentQuestManager = QuestManager;
}

UQuestManager* UQSUtility::GetQuestManager()
{
	return CurrentQuestManager;
}

void UQSUtility::AchieveTask(const UTaskData* TaskDataKey)
{
	UQuestManager* QuestManager = GetQuestManager();
	
	if(!QuestManager)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot achieve the task."));
		return;
	}

	QuestManager->AchieveTaskInActiveQuests(TaskDataKey);
}

void UQSUtility::ActivateQuest(const UQuestData* QuestDataKey)
{
	UQuestManager* QuestManager = GetQuestManager();
	
	if(!QuestManager)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot activate the quest."));
		return;
	}

	QuestManager->AddToActiveQuests(QuestDataKey);
}

void UQSUtility::DeactivateQuest(const UQuestData* QuestDataKey)
{
	UQuestManager* QuestManager = GetQuestManager();
	
	if(!QuestManager)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot deactivate the quest."));
		return;
	}

	QuestManager->AddToInactiveQuests(QuestDataKey);
}

void UQSUtility::CompleteQuestNow(const UQuestData* QuestDataKey)
{
	UQuestManager* QuestManager = GetQuestManager();
	
	if(!QuestManager)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot complete the quest."));
		return;
	}

	QuestManager->AddToCompletedQuests(QuestDataKey, true);
}

void UQSUtility::TrackQuest(const UQuestData* QuestDataKey)
{
	UQuestManager* QuestManager = GetQuestManager();
	
	if(!QuestManager)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot track the quest."));
		return;
	}

	QuestManager->TrackQuest(QuestDataKey);
}
