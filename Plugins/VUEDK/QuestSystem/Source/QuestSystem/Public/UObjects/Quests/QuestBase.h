// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataAssets/QuestData.h"
#include "Data/Enums/QuestStatus.h"
#include "Data/Structs/QuestEntryData.h"
#include "Data/Structs/QuestSaveData.h"
#include "UObject/Object.h"
#include "QuestBase.generated.h"

class UTaskBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);

UCLASS(Abstract, BlueprintType)
class QUESTSYSTEM_API UQuestBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestData* QuestData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<UTaskData*, UTaskBase*> AllTasks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsQuestCompleted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnQuestCompleted OnQuestCompleted;

private:
	EQuestType QuestType;
	EQuestStatus QuestStatus;
	FQuestEntryData QuestEntryData;

public:
	virtual void Init(UQuestData* InitData, const FQuestEntryData& EntryData);

	UFUNCTION(BlueprintCallable)
	virtual FQuestSaveData CreateQuestSaveData() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void LoadSaveData(FQuestSaveData QuestSaveData);

	UFUNCTION(BlueprintCallable)
	virtual void AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve = false);

	UFUNCTION(BlueprintCallable)
	void AchieveAllTasks(const bool bFullyAchieve = false) const;

	UFUNCTION(BlueprintCallable)
	bool IsTaskAchieved(const UTaskData* TaskDataKey) const;

	UFUNCTION(BlueprintCallable)
	UTaskBase* GetTask(const UTaskData* TaskDataKey) const;

	UFUNCTION(BlueprintCallable)
	UTaskBase* GetTaskByFName(const FName TaskName) const;

	void SetQuestStatus(EQuestStatus Status);

	UFUNCTION(BlueprintPure)
	EQuestStatus GetQuestStatus() const;

	UFUNCTION(BlueprintPure)
	EQuestType GetQuestType() const;

	UFUNCTION(BlueprintCallable)
	virtual void ResetQuest();

protected:
	bool AreAllTasksAchieved() const;
};
