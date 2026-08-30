// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataAssets/TaskData.h"
#include "UObject/Object.h"
#include "TaskBase.generated.h"

class UQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskAchieved);

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class QUESTSYSTEM_API UTaskBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTaskData* TaskData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestBase* RelatedQuest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAchieved;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnTaskAchieved OnTaskAchieved;
	
public:
	virtual void Init(UTaskData* InitData, UQuestBase* Quest);

	UFUNCTION(BlueprintCallable)
	virtual void AchieveTask(bool bFullyAchieve = false);
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetTask();

protected:
	void TriggerAchievement();
};
