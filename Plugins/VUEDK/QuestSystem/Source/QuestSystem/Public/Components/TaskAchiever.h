// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DataAssets/TaskData.h"
#include "TaskAchiever.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UTaskAchiever : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskData* TaskToAchieve;

public:
	UTaskAchiever();

	UFUNCTION(BlueprintCallable)
	void AchieveTask();
};
