// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/Quests/QuestBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QSFactory.generated.h"

UCLASS()
class QUESTSYSTEM_API UQSFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a quest instance based on the provided quest data and entry data.
	 * @param QuestData - The data asset describing the quest to create.
	 * @param QuestEntryData - The entry data for the quest instance.
	 * @return A pointer to the created UQuestBase instance.
	 */
	static UQuestBase* CreateQuestByType(UQuestData* QuestData, const FQuestEntryData& QuestEntryData);
	
	/**
	 * Creates a task instance based on the provided quest, task data, and task type.
	 * @param RelatedQuest - The quest this task will be related to.
	 * @param TaskData - The data asset describing the task to create.
	 * @param TaskType - The type of the task to create.
	 * @return A pointer to the created UTaskBase instance.
	 */
	static UTaskBase* CreateTaskByType(UQuestBase* RelatedQuest, UTaskData* TaskData, const ETaskType TaskType);
};
