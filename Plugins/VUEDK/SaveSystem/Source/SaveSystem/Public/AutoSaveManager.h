// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/AutoSaveManagerData.h"
#include "AutoSaveManager.generated.h"

UCLASS()
class SAVESYSTEM_API UAutoSaveManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USaveManager* CurrentSaveManager;
	FAutoSaveManagerData AutoSaveData;
	FTimerHandle AutoSaveTimerHandle;
	bool bIsPaused;

public:
	/**
	 * Initializes the auto save manager with the given save manager and auto save data.
	 * @param SaveManager - The save manager to use for auto saving.
	 * @param InAutoSaveData - The auto save configuration data.
	 */
	void Init(USaveManager* SaveManager, const FAutoSaveManagerData& InAutoSaveData);

	/**
	 * Pauses the auto save timer, preventing automatic saves until unpaused.
	 */
	UFUNCTION(BlueprintCallable)
	void PauseAutoSave();

	/**
	 * Unpauses the auto save timer, resuming automatic saves.
	 */
	UFUNCTION(BlueprintCallable)
	void UnpauseAutoSave();

	/**
	 * Checks if auto save is currently paused.
	 * @return True if auto save is paused, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAutoSavePaused() const;
	
private:
	/**
	 * Performs the auto save operation. Called by the timer.
	 */
	void AutoSave() const;

	/**
	 * Gets the world timer manager for scheduling auto save operations.
	 * @return Pointer to the FTimerManager, or nullptr if not available.
	 */
	FTimerManager* GetWorldTimerManager() const;
};
