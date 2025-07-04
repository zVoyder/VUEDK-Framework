// Copyright VUEDK, Inc. All Rights Reserved.

#include "UObjects/Quests/QuestParallel.h"
#include "UObjects/Tasks/TaskBase.h"

void UQuestParallel::AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve)
{
	if (bIsQuestCompleted) return;
	
	UTaskBase* Task = GetTask(TaskDataKey);
	if (!Task) return;
	
	Task->AchieveTask(bFullyAchieve);
	bIsQuestCompleted = AreAllTasksAchieved();

	if (bIsQuestCompleted)
		OnQuestCompleted.Broadcast();
}
