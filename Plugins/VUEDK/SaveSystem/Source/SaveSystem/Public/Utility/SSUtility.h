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
	static USaveManager* CurrentSaveManager;
	static UAutoSaveManager* AutoSaveManager;
	
public:
	/**
	 * Initializes the utility with a save manager.
	 * @param SaveManager - The save manager to initialize with.
	 */
	static void Init(USaveManager* SaveManager);

	/**
	 * Sets the auto save manager.
	 * @param InAutoSaveManager - The auto save manager to set.
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
	 * @param Instigator - The object that initiated the save.
	 * @param bOverrideCurrentSlot - If true, overrides the current slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	/**
	 * Performs a shared save.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SharedSave(UObject* Instigator);

	/**
	 * Performs a manual and shared save operation.
	 * @param Instigator - The object that initiated the save.
	 * @param bOverrideCurrentSlot - If true, overrides the current slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ManualAndSharedSave(UObject* Instigator, bool bOverrideCurrentSlot = false);

	/**
	 * Selects a slot by name and saves to it.
	 * @param SlotName - The name of the slot to save to.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SelectAndSaveOnSlot(const FString& SlotName, UObject* Instigator);
	
	/**
	 * Loads the currently selected slot.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadSelectedSlot(UObject* Instigator);

	/**
	 * Loads the shared slot.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadSharedSlot(UObject* Instigator);

	/**
	 * Loads both the selected slot and the shared slot.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadSelectedSlotAndSharedSlot(UObject* Instigator);
	
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
	 * Sets the master save ID.
	 * @param SaveMasterID - The master save ID to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SetSaveMasterID(const FName& SaveMasterID);
	
	/**
	 * Gets the master save ID.
	 * @return The current master save ID.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static FName GetSaveMasterID();
	
private:
	/**
	 * Checks if the utility is in a valid state for operations.
	 * @return True if valid, false otherwise.
	 */
	static bool Check();
};
