// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskData.h"
#include "CountTaskData.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEM_API UCountTaskData : public UTaskData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 CountToAchieve;

public:
	virtual ETaskType GetTaskType() const override;
};
