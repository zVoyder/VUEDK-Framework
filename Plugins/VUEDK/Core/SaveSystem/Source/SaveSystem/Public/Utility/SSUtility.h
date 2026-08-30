// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutoSaveManager.h"
#include "SaveManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSUtility.generated.h"

UCLASS()
class SAVESYSTEM_API USSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<USaveManager> SaveManager;
	static TWeakObjectPtr<UAutoSaveManager> AutoSaveManager;
	
public:
	/**
	 * Initializes the utility with a save manager.
	 * @param InSaveManager The save manager to initialize with.
	 */
	static void Init(USaveManager* InSaveManager);

	/**
	 * Sets the auto save manager.
	 * @param InAutoSaveManager The auto save manager to set.
	 */
	static void SetAutoSaveManager(UAutoSaveManager* InAutoSaveManager);

	/**
	 * Gets the current save manager.
	 * @return The current save manager.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static USaveManager* GetSaveManager();

	/**
	 * Gets the current auto save manager.
	 * @return The current auto save manager.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static UAutoSaveManager* GetAutoSaveManager();
	
	/**
	 * Gets the current save game instance.
	 * @return The current save game instance.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static UDefaultSaveGame* GetSaveGame();

	/**
	 * Gets the shared save game instance.
	 * @return The shared save game instance.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static UDefaultSaveGame* GetSharedSaveGame();

	/**
	 * Starts a new save game.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void StartNewSaveGame();
	
	/**
	 * Performs a manual save.
	 * @param Instigator The object that initiated the save.
	 * @param bOverrideCurrentSlot If true, overrides the current slot.
	 * @param bAsync Whether to perform the save asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false, const bool bAsync = true);

	/**
	 * Performs a shared save.
	 * @param Instigator The object that initiated the save.
	 * @param bAsync Whether to perform the save asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SharedSave(UObject* Instigator, const bool bAsync = true);

	/**
	 * Performs a manual and shared save operation.
	 * @param Instigator The object that initiated the save.
	 * @param bOverrideCurrentSlot If true, overrides the current slot.
	 * @param bAsync Whether to perform the save asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot = false, const bool bAsync = true);

	/**
	 * Selects a slot by name and saves to it.
	 * @param SlotName The name of the slot to save to.
	 * @param Instigator The object that initiated the save.
	 * @param bAsync Whether to perform the save asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SelectAndSaveOnSlot(const FString& SlotName, UObject* Instigator, const bool bAsync = true);

	/**
	 * Selects a slot by name and loads from it.
	 * @param SlotName The name of the slot to load from.
	 * @param Instigator The object that initiated the load.
	 * @param bAsync Whether to perform the load asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SelectAndLoadFromSlot(const FString& SlotName, UObject* Instigator, const bool bAsync = true);
	
	/**
	 * Loads the currently selected slot.
	 * @param Instigator The object that initiated the load.
	 * @param bAsync Whether to perform the load asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadSelectedSlot(UObject* Instigator, const bool bAsync = true);

	/**
	 * Loads the shared slot.
	 * @param Instigator The object that initiated the load.
	 * @param bAsync Whether to perform the load asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadSharedSlot(UObject* Instigator, const bool bAsync = true);

	/**
	 * Loads both the selected slot and the shared slot.
	 * @param Instigator The object that initiated the load.
	 * @param bAsync Whether to perform the load asynchronously or synchronously.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadSelectedSlotAndSharedSlot(UObject* Instigator, const bool bAsync = true);

	/**
	 * Pauses the auto save timer, preventing automatic saves until unpaused.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void PauseAutoSave();

	/**
	 * Unpauses the auto save timer, resuming automatic saves.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void UnpauseAutoSave();
	
	/**
	 * Starts the timer for tracking played time.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void StartTimePlayedTimer();

	/**
	 * Checks if the instance has ever loaded any save.
	 * @return True if the instance has loaded, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool HasInstanceEverLoaded();

	/**
	 * Checks if the instance has ever saved any data.
	 * @return True if the instance has saved, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool HasInstanceEverSaved();

	/**
	 * Checks if the shared instance has ever loaded any save.
	 * @return True if the shared instance has loaded, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool HasSharedInstanceEverLoaded();

	/**
	 * Checks if the shared instance has ever saved any data.
	 * @return True if the shared instance has saved, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool HasSharedInstanceEverSaved();

	/**
	 * Checks if the save manager has started a new save game.
	 * @return True if it is a new save game, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsNewSaveGame();

private:
	/**
	 * Checks if the utility is in a valid state for operations.
	 * @return True if valid, false otherwise.
	 */
	static bool Check();
};
