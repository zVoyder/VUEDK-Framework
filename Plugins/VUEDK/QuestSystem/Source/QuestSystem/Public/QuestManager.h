// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObjects/Quests/QuestBase.h"
#include "Data/DataAssets/QuestData.h"
#include "Data/DataAssets/QuestLogData.h"
#include "Data/Structs/QuestLogSaveData.h"
#include "QuestManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyQuestCompleted,
	const UQuestBase*, Quest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAnyTaskAchieved,
	const UQuestBase*, Quest,
	const UTaskBase*, Task);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnQuestTracked,
	const UQuestBase*, Quest);

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UQuestLogData* QuestLogData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestBase* TrackedQuest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<UQuestData*, UQuestBase*> AllQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestBase*> ActiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestBase*> InactiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestBase*> CompletedQuests;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyQuestCompleted OnAnyQuestCompleted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyTaskAchieved OnAnyTaskAchieved;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnQuestTracked OnQuestTracked;

public:
	UQuestManager();

	virtual void BeginPlay() override;

	/**
	 * Initializes the quest manager, loading quests from the QuestLogData.
	 * This function should be called after the component is created.
	 */
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	virtual FQuestLogSaveData CreateSaveData();

	UFUNCTION(BlueprintCallable)
	virtual void LoadSaveData(FQuestLogSaveData QuestLogSaveData);

	/**
	 * Tracks the specified quest, making it the currently tracked quest.
	 * @param QuestDataKey - The quest data asset to track.
	 */
	UFUNCTION(BlueprintCallable)
	void TrackQuest(const UQuestData* QuestDataKey);

	/**
	 * Achieves the specified task in all active quests that contain it.
	 * @param TaskDataKey - The task data asset to achieve.
	 */
	UFUNCTION(BlueprintCallable)
	void AchieveTaskInActiveQuests(const UTaskData* TaskDataKey);

	/**
	 * Achieves the specified task in a specific quest.
	 * @param QuestDataKey - The quest data asset containing the task.
	 * @param TaskDataKey - The task data asset to achieve.
	 */
	UFUNCTION(BlueprintCallable)
	void AchieveTaskInQuest(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey);

	/**
	 * Adds the specified quest to the active quests list.
	 * @param QuestDataKey - The quest data asset to add.
	 */
	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddToActiveQuests(const UQuestData* QuestDataKey);

	/**
	 * Adds the specified quest to the inactive quests list.
	 * @param QuestDataKey - The quest data asset to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddToInactiveQuests(const UQuestData* QuestDataKey);

	/**
	 * Adds the specified quest to the completed quests list, optionally achieving all its tasks.
	 * @param QuestDataKey - The quest data asset to add.
	 * @param bAchieveAllTasks - If true, all tasks in the quest will be marked as achieved.
	 */
	UFUNCTION(BlueprintCallable)
	void AddToCompletedQuests(const UQuestData* QuestDataKey, bool bAchieveAllTasks = false);

	/**
	 * Checks if the specified quest is in the completed quests list.
	 * @param QuestDataKey - The quest data asset to check.
	 * @return true if the quest is completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsInCompletedQuestsList(const UQuestData* QuestDataKey) const;

	/**
	 * Checks if the specified quest is in the active quests list.
	 * @param QuestDataKey - The quest data asset to check.
	 * @return true if the quest is active, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsInActiveQuestsList(const UQuestData* QuestDataKey) const;

	/**
	 * Checks if the specified quest is in the inactive quests list.
	 * @param QuestDataKey - The quest data asset to check.
	 * @return true if the quest is inactive, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsInInactiveQuestsList(const UQuestData* QuestDataKey) const;

	/**
	 * Checks if a specific task in a quest has been achieved.
	 * @param QuestDataKey - The quest data asset.
	 * @param TaskDataKey - The task data asset to check.
	 * @return true if the task is achieved, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const;

	/**
	 * Retrieves the quest instance associated with the specified quest data.
	 * @param QuestDataKey - The quest data asset to retrieve.
	 * @return Pointer to the quest instance if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	UQuestBase* GetQuest(const UQuestData* QuestDataKey) const;

	/**
	 * Retrieves all quests that match the specified filter.
	 * @param QuestFilterData - The filter data to apply.
	 * @return Array of quest instances matching the filter.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<UQuestBase*> GetQuestsByFilter(const UQuestFilterData* QuestFilterData) const;

	/**
	 * Resets the entire quest log, clearing all quests.
	 */
	void ResetQuestLog();
	
	/**
	 * Retrieves a quest instance by its FName identifier.
	 * @param QuestFName - The FName of the quest to retrieve.
	 * @return Pointer to the quest instance if found, nullptr otherwise.
	 */
	UQuestBase* GetQuestByFName(const FName QuestFName) const;
	
	/**
	 * Tracks a quest by its FName identifier.
	 * @param QuestFName - The FName of the quest to track.
	 */
	void TrackQuestByFName(const FName QuestFName);

#if WITH_EDITOR
	/**
	 * Logs all quests to the output log (Editor only).
	 */
	UFUNCTION(BlueprintCallable)
	void LogAllQuests() const;
#endif

protected:
	/**
	 * Adds a quest to the quest log with the specified entry data.
	 * @param QuestData - The quest data asset to add.
	 * @param QuestEntryData - The entry data for the quest.
	 */
	void AddQuest(UQuestData* QuestData, const FQuestEntryData QuestEntryData);
	
	/**
	 * Removes a quest from the quest log.
	 * @param QuestDataKey - The quest data asset to remove.
	 */
	void RemoveQuest(const UQuestData* QuestDataKey);
};
