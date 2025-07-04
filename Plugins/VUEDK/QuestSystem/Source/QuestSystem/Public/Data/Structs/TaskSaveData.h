// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FTaskSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAchieved;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentAchieveCount;

	FTaskSaveData(): bIsAchieved(false),
	                 CurrentAchieveCount(0)
	{
	}
};
