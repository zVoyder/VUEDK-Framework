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
	/**
	 * Initializes the quest with the provided quest data and entry data.
	 * @param InitData - The quest data asset to initialize from.
	 * @param EntryData - The entry data for this quest instance.
	 */
	virtual void Init(UQuestData* InitData, const FQuestEntryData& EntryData);

	/**
	 * Creates a save data structure representing the current state of the quest.
	 * @return FQuestSaveData containing all relevant quest information for saving.
	 */
	UFUNCTION(BlueprintCallable)
	virtual FQuestSaveData CreateQuestSaveData() const;
	
	/**
	 * Loads quest state from the provided save data.
	 * @param QuestSaveData - The save data to load from.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void LoadSaveData(FQuestSaveData QuestSaveData);

	/**
	 * Marks a specific quest task as achieved.
	 * @param TaskDataKey - The task to achieve.
	 * @param bFullyAchieve - If true, fully completes the task.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve = false);

	/**
	 * Achieves all tasks in the quest.
	 * @param bFullyAchieve - If true, fully completes all tasks.
	 */
	UFUNCTION(BlueprintCallable)
	void AchieveAllTasks(const bool bFullyAchieve = false) const;

	/**
	 * Checks if a specific task has been achieved.
	 * @param TaskDataKey - The task to check.
	 * @return true if the task is achieved, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsTaskAchieved(const UTaskData* TaskDataKey) const;

	/**
	 * Retrieves a task by its data key.
	 * @param TaskDataKey - The key of the task to retrieve.
	 * @return Pointer to the task if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	UTaskBase* GetTask(const UTaskData* TaskDataKey) const;

	/**
	 * Retrieves a task by its name.
	 * @param TaskName - The name of the task to retrieve.
	 * @return Pointer to the task if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	UTaskBase* GetTaskByFName(const FName TaskName) const;

	/**
	 * Sets the current status of the quest.
	 * @param Status - The new quest status to set.
	 */
	void SetQuestStatus(EQuestStatus Status);

	/**
	 * Gets the current status of the quest.
	 * @return The current quest status.
	 */
	UFUNCTION(BlueprintPure)
	EQuestStatus GetQuestStatus() const;

	/**
	 * Gets the type of the quest.
	 * @return The quest type.
	 */
	UFUNCTION(BlueprintPure)
	EQuestType GetQuestType() const;

	/**
	 * Resets the quest to its initial state.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ResetQuest();

protected:
	/**
	* Checks if all tasks in the quest have been achieved.
	* @return true if all tasks are achieved, false otherwise.
	*/
	bool AreAllTasksAchieved() const;
};
