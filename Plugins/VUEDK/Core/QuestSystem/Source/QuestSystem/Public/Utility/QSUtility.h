// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QSUtility.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType)
class QUESTSYSTEM_API UQSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<UQuestManager> CurrentQuestManager;
	
public:
	/**
	 * Initializes the Quest System with the given QuestManager.
	 * @param QuestManager The QuestManager to initialize with.
	 */
	static void Init(UQuestManager* QuestManager);
	
	/**
	 * Gets the found QuestManager.
	 * @return QuestManager.
	 */
	UFUNCTION(BlueprintPure, Category = "Gameplay|Quest System")
	static UQuestManager* GetQuestManager();

	/**
	 * Activates a quest and tracks it immediately.
	 * @param QuestDataKey The quest to activate and track.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Quest System")
	static void ActivateAndTrackQuest(const UQuestData* QuestDataKey);

	/**
	 * Achieves a task in the active quests list.
	 * @param TaskDataKey The task to achieve.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Quest System")
	static void AchieveTask(const UTaskData* TaskDataKey);

	/**
	 * Puts the quest in the active quests list.
	 * @param QuestDataKey The quest to activate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Quest System")
	static void ActivateQuest(const UQuestData* QuestDataKey);

	/**
	 * Puts the quest in the inactive quests list.
	 * @param QuestDataKey The quest to deactivate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Quest System")
	static void DeactivateQuest(const UQuestData* QuestDataKey);

	/**
	 * Puts the quest in the completed quests list and achieves all its tasks.
	 * @param QuestDataKey The quest to complete.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Quest System")
	static void CompleteQuestNow(const UQuestData* QuestDataKey);

	/**
	 * Tracks a quest.
	 * @param QuestDataKey The quest to track.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Quest System")
	static void TrackQuest(const UQuestData* QuestDataKey);
};
