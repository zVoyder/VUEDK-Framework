// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/QSUtility.h"
#include "QuestManager.h"
#include "QuestSystem.h"

TWeakObjectPtr<UQuestManager> UQSUtility::CurrentQuestManager = nullptr;

void UQSUtility::Init(UQuestManager* QuestManager)
{
	CurrentQuestManager = QuestManager;
}

UQuestManager* UQSUtility::GetQuestManager()
{
	return CurrentQuestManager.Get();
}

void UQSUtility::ActivateAndTrackQuest(const UQuestData* QuestDataKey)
{
	ActivateQuest(QuestDataKey);
	TrackQuest(QuestDataKey);
}

void UQSUtility::AchieveTask(const UTaskData* TaskDataKey)
{
	if(!CurrentQuestManager.IsValid())
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot achieve the task."));
		return;
	}

	CurrentQuestManager.Get()->AchieveTaskInActiveQuests(TaskDataKey);
}

void UQSUtility::ActivateQuest(const UQuestData* QuestDataKey)
{
	if(!CurrentQuestManager.IsValid())
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot activate the quest."));
		return;
	}

	CurrentQuestManager.Get()->AddToActiveQuests(QuestDataKey);
}

void UQSUtility::DeactivateQuest(const UQuestData* QuestDataKey)
{
	if(!CurrentQuestManager.IsValid())
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot deactivate the quest."));
		return;
	}

	CurrentQuestManager.Get()->AddToInactiveQuests(QuestDataKey);
}

void UQSUtility::CompleteQuestNow(const UQuestData* QuestDataKey)
{
	if(!CurrentQuestManager.IsValid())
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot complete the quest."));
		return;
	}

	CurrentQuestManager.Get()->AddToCompletedQuests(QuestDataKey, true);
}

void UQSUtility::TrackQuest(const UQuestData* QuestDataKey)
{
	if(!CurrentQuestManager.IsValid())
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestManager is null. Cannot track the quest."));
		return;
	}

	CurrentQuestManager.Get()->TrackQuest(QuestDataKey);
}
