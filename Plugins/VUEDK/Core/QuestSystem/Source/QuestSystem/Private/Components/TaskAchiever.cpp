// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/TaskAchiever.h"
#include "QuestSystem.h"
#include "Utility/QSUtility.h"

UTaskAchiever::UTaskAchiever(): TaskToAchieve(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTaskAchiever::AchieveTask()
{
	if (!TaskToAchieve)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("TaskToAchieve is null. Cannot achieve the task."));
		return;
	}
	
	UQSUtility::AchieveTask(TaskToAchieve);
}
