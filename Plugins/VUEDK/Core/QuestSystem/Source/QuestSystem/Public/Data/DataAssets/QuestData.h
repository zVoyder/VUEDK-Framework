// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFilterData.h"
#include "TaskData.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEM_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UQuestFilterData* QuestFilter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UTaskData*> TasksData;
};
