// Copyright VUEDK, Inc. All Rights Reserved.

#include "QuestManager.h"
#include "Utility/QSFactory.h"
#include "Utility/QSUtility.h"
#include "UObjects/Quests/QuestBase.h"
#include "UObjects/Tasks/TaskBase.h"

UQuestManager::UQuestManager(): QuestLogData(nullptr), TrackedQuest(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
	UQSUtility::Init(this); // Initialize the utility class here, as it needs the QuestManager reference
}

void UQuestManager::Init()
{
	if (!QuestLogData || QuestLogData->QuestEntries.IsEmpty())
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestLogData is null or empty. Cannot initialize the QuestLog."));
		return;
	}

	for (const TTuple<UQuestData*, FQuestEntryData> QuestEntry : QuestLogData->QuestEntries)
	{
		if (!QuestEntry.Key)
		{
			UE_LOG(LogQuestSystem, Warning, TEXT("A QuestData in the QuestLogData is null. Skipping..."));
			continue;
		}

		AddQuest(QuestEntry.Key, QuestEntry.Value);
	}

	TrackQuest(QuestLogData->FirstTrackedQuest);
	UE_LOG(LogQuestSystem, Display, TEXT("QuestLog initialized."));
}

FQuestLogSaveData UQuestManager::CreateSaveData()
{
	FQuestLogSaveData QuestLogSaveData;

	for (const TTuple<UQuestData*, UQuestBase*> QuestTuple : AllQuests)
	{
		FQuestSaveData QuestSaveData = QuestTuple.Value->CreateQuestSaveData();
		QuestLogSaveData.Quests.Add(QuestTuple.Key->GetFName(), QuestSaveData);
	}
	
	QuestLogSaveData.TrackedQuestFName = TrackedQuest ? TrackedQuest->QuestData->GetFName() : NAME_None;
	return QuestLogSaveData;
}

void UQuestManager::LoadSaveData(FQuestLogSaveData QuestLogSaveData)
{
	ResetQuestLog();
	for (const TTuple<FName, FQuestSaveData> QuestsData : QuestLogSaveData.Quests)
	{
		UQuestBase* Quest = GetQuestByFName(QuestsData.Key);
		if (!Quest) continue; // If the quest is not found, skip it

		Quest->LoadSaveData(QuestsData.Value);
		
		switch (QuestsData.Value.QuestStatus)
		{
			case EQuestStatus::Active:
				AddToActiveQuests(Quest->QuestData);
				break;
		
			case EQuestStatus::Inactive:
				AddToInactiveQuests(Quest->QuestData);
				break;
		
			case EQuestStatus::Completed:
				AddToCompletedQuests(Quest->QuestData);
				break;
		
			default: ;
		}
	}
	
	TrackedQuest = nullptr;
	if (QuestLogSaveData.TrackedQuestFName != NAME_None)
		TrackQuestByFName(QuestLogSaveData.TrackedQuestFName);
}

void UQuestManager::TrackQuest(const UQuestData* QuestDataKey)
{
	UQuestBase* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	TrackedQuest = Quest;
	OnQuestTracked.Broadcast(TrackedQuest);
}

void UQuestManager::AchieveTaskInActiveQuests(const UTaskData* TaskDataKey)
{
	TArray<UQuestBase*> TempActiveQuests = ActiveQuests;
	// Shallow copy, avoid modifying the original array while iterating

	for (const UQuestBase* Quest : TempActiveQuests)
		AchieveTaskInQuest(Quest->QuestData, TaskDataKey);
}

void UQuestManager::AchieveTaskInQuest(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey)
{
	UQuestBase* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	const UTaskBase* Task = Quest->GetTask(TaskDataKey);
	if (!Task || Task->bIsAchieved) return;

	Quest->AchieveQuestTask(TaskDataKey);
	OnAnyTaskAchieved.Broadcast(Quest, Task);

	if (Quest->bIsQuestCompleted)
	{
		AddToCompletedQuests(Quest->QuestData);
		OnAnyQuestCompleted.Broadcast(Quest);
	}
}

void UQuestManager::AddToActiveQuests(const UQuestData* QuestDataKey)
{
	UQuestBase* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	Quest->SetQuestStatus(EQuestStatus::Active);
	ActiveQuests.Add(Quest);
	InactiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestManager::AddToInactiveQuests(const UQuestData* QuestDataKey)
{
	UQuestBase* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	Quest->SetQuestStatus(EQuestStatus::Inactive);
	InactiveQuests.Add(Quest);
	ActiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestManager::AddToCompletedQuests(const UQuestData* QuestDataKey, const bool bAchieveAllTasks)
{
	UQuestBase* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	if (bAchieveAllTasks)
		Quest->AchieveAllTasks(true);

	Quest->SetQuestStatus(EQuestStatus::Completed);
	CompletedQuests.Add(Quest);
	ActiveQuests.Remove(Quest);
	InactiveQuests.Remove(Quest);
}

bool UQuestManager::IsInCompletedQuestsList(const UQuestData* QuestDataKey) const
{
	return CompletedQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsInActiveQuestsList(const UQuestData* QuestDataKey) const
{
	return ActiveQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsInInactiveQuestsList(const UQuestData* QuestDataKey) const
{
	return InactiveQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const
{
	const UQuestBase* Quest = GetQuest(QuestDataKey);

	if (!Quest) return false;
	if (!IsInActiveQuestsList(QuestDataKey))
	{
		if (IsInInactiveQuestsList(QuestDataKey))
			return false;

		if (IsInCompletedQuestsList(QuestDataKey))
			return true;
	}

	return Quest->IsTaskAchieved(TaskDataKey);
}

UQuestBase* UQuestManager::GetQuest(const UQuestData* QuestDataKey) const
{
	return AllQuests.FindRef(QuestDataKey);
}

TArray<UQuestBase*> UQuestManager::GetQuestsByFilter(const UQuestFilterData* QuestFilterData) const
{
	TArray<UQuestBase*> FilteredQuests;

	for (const auto& QuestTuple : AllQuests)
	{
		if (QuestTuple.Key->QuestFilter == QuestFilterData)
			FilteredQuests.Add(QuestTuple.Value);
	}

	return FilteredQuests;
}

void UQuestManager::ResetQuestLog()
{
	ActiveQuests.Empty();
	InactiveQuests.Empty();
	CompletedQuests.Empty();
	TrackedQuest = nullptr;
	Init();
}

UQuestBase* UQuestManager::GetQuestByFName(const FName QuestFName) const
{
	for (const auto& QuestTuple : AllQuests)
	{
		if (QuestTuple.Key->GetFName() == QuestFName)
			return QuestTuple.Value;
	}

	return nullptr;
}

void UQuestManager::TrackQuestByFName(const FName QuestFName)
{
	UQuestBase* Quest = GetQuestByFName(QuestFName);
	if (!Quest) return;

	TrackedQuest = Quest;
	OnQuestTracked.Broadcast(TrackedQuest);
}

#if WITH_EDITOR
void UQuestManager::LogAllQuests() const
{
	for( const auto& QuestTuple : AllQuests)
	{
		UE_LOG(LogQuestSystem, Display, TEXT("Quest: %s"), *QuestTuple.Key->QuestName);

		for( const auto& TaskTuple : QuestTuple.Value->AllTasks)
		{
			const UTaskData* TaskData = TaskTuple.Key;

			UE_LOG(LogQuestSystem, Display, TEXT("Task: %s"), *TaskData->Description);
			UE_LOG(LogQuestSystem, Display, TEXT("Task is achieved: %s"), TaskTuple.Value->bIsAchieved ? TEXT("true") : TEXT("false") );
		}
	}
}
#endif

void UQuestManager::AddQuest(UQuestData* QuestData, const FQuestEntryData QuestEntryData)
{
	if (!QuestData)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("QuestData is null. Cannot add the quest."));
		return;
	}

	UQuestBase* Quest = UQSFactory::CreateQuestByType(QuestData, QuestEntryData);
	if (!Quest)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("Quest is null. Cannot add the quest."));
		return;
	}

	// QuestData->GetUniqueID(); Could use a unique ID for the key
	AllQuests.Add(QuestData, Quest);

	switch (QuestEntryData.InitialQuestStatus)
	{
	case EQuestStatus::Active:
		AddToActiveQuests(QuestData);
		break;

	case EQuestStatus::Inactive:
		AddToInactiveQuests(QuestData);
		break;

	case EQuestStatus::Completed:
		AddToCompletedQuests(QuestData, true);
		break;

	default: ;
	}
}

void UQuestManager::RemoveQuest(const UQuestData* QuestDataKey)
{
	UQuestBase* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	AllQuests.Remove(QuestDataKey);
	ActiveQuests.Remove(Quest);
	InactiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}
