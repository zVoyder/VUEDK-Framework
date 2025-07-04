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
	static UQuestBase* CreateQuestByType(UQuestData* QuestData, const FQuestEntryData& QuestEntryData);
	
	static UTaskBase* CreateTaskByType(UQuestBase* RelatedQuest, UTaskData* TaskData, const ETaskType TaskType);
};
