// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "Data/Saves/SlotInfoItem.h"
#include "Data/Saves/SlotInfos.h"
#include "Data/Structs/SaveManagerData.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogSaveSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnPrepareSave,
	UDefaultSaveGame*, SaveGameData,
	USlotInfoItem*, SlotInfoItem,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnPrepareSharedSave,
	UDefaultSaveGame*, SaveGameData,
	USlotInfoItem*, SlotInfoItem,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareLoad,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareSharedLoad,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnSharedSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnSharedLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPendingSharedSave,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPendingSharedLoad,
	UObject*, Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnNewSaveGame
);

UCLASS(NotBlueprintable, BlueprintType)
class SAVESYSTEM_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<UDefaultSaveGame> SaveGameClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<UDefaultSaveGame> SharedSaveGameClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<USlotInfoItem> SlotInfoItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<USlotInfoItem> SharedSlotInfoItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double ElapsedTimePlayed;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareSave OnPrepareSave;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareLoad OnPrepareLoad;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSaveGame OnSaveGame;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnLoadGame OnLoadGame;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnNewSaveGame OnNewSaveGame;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareSharedSave OnPrepareSharedSave;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareSharedLoad OnPrepareSharedLoad;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSharedSaveGame OnSharedSaveGame;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSharedLoadGame OnSharedLoadGame;

private:
	UPROPERTY()
	UDefaultSaveGame* SlotSaveGameInstance;
	UPROPERTY()
	UDefaultSaveGame* SharedSaveGameInstance;
	UPROPERTY()
	USlotInfos* CurrentSlotInfos;
	UPROPERTY()
	USlotInfoItem* CurrentSlotInfoItem;
	UPROPERTY()
	USlotInfoItem* CurrentSharedSlotInfoItem;
	UPROPERTY()
	UObject* CurrentInstigator;
	bool bIsSharedSaving;
	bool bIsSharedLoading;
	bool bIsLoading;
	bool bIsSaving;
	bool bHasEverLoaded;
	bool bHasEverSaved;
	bool bHasEverSharedSaved;
	bool bHasEverSharedLoaded;
	FName PreviousSlotNameKey;
	bool bSaveAsNewGame;
	FTimerHandle TimePlayedTimerHandle;
	FName SaveMasterID;

	FOnPendingSharedSave OnPendingSharedSave;
	FOnPendingSharedLoad OnPendingSharedLoad;

public:
	USaveManager();

	/**
	 * Initializes the SaveManager with the provided SaveManagerData.
	 * @param SaveManagerData - The data structure containing configuration for the SaveManager.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(const FSaveManagerData SaveManagerData);

	/**
	 * Gets the current save game instance.
	 * @return The current save game instance.
	 */
	UFUNCTION(BlueprintPure)
	UDefaultSaveGame* GetSaveGameInstance() const;

	/**
	 * Gets the shared save game instance.
	 * @return The shared save game instance.
	 */
	UFUNCTION(BlueprintPure)
	UDefaultSaveGame* GetSharedSaveGameInstance() const;

	/**
	 * Gets the current slot info item.
	 * @param SlotName - The name of the slot to retrieve the info for.
	 * @return The slot info item for the specified slot name.
	 */
	UFUNCTION(BlueprintCallable)
	bool DeleteSlot(const FName SlotName);

	/**
	 * Deletes all save slots.
	 */
	UFUNCTION(BlueprintCallable)
	void DeleteAllSlots();

	/**
	 * Starts a new save game instance.
	 */
	UFUNCTION(BlueprintCallable)
	void StartNewSaveGame();

	/**
	 * Manually saves the current game state.
	 * @param Instigator - The object that initiated the save.
	 * @param bOverrideCurrentSlot - Whether to override the current save slot.
	 */
	UFUNCTION(BlueprintCallable)
	void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	/**
	 * Saves the current game state to a shared slot.
	 * @param Instigator - The object that initiated the shared save.
	 */
	UFUNCTION(Blueprintable)
	void SharedSave(UObject* Instigator);

	/**
	 * Manually saves the current game state and also saves to a shared slot.
	 * @param Instigator - The object that initiated the save.
	 * @param bOverrideCurrentSlot - Whether to override the current save slot.
	 */
	UFUNCTION(BlueprintCallable)
	void ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	/**
	 * Selects a save slot and saves the current game state to it.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSelectedSlotAndSharedSlot(UObject* Instigator);

	/**
	 * Loads the selected save slot.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSelectedSlot(UObject* Instigator);

	/**
	 * Loads a shared save slot.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSharedSlot(UObject* Instigator);

	/**
	 * Loads the selected save slot and a shared save slot.
	 * @return True if the load was successful, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FSlotInfoData> GetSaveInfos() const;

	/**
	 * Gets the current slot info item.
	 * @param OutIsLoading - Whether the save manager is currently loading.
	 * @param OutIsSaving - Whether the save manager is currently saving.
	 * @return True if the status was retrieved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool GetStatus(bool& OutIsLoading, bool& OutIsSaving) const;

	/**
	 * Checks if the save manager is currently loading a save.
	 * @return True if loading, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasInstanceEverSaved() const;

	/**
	 * Checks if the save manager has ever loaded a save.
	 * @return True if the instance has ever loaded, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasInstanceEverLoaded() const;

	/**
	 * Checks if the shared save instance has ever saved.
	 * @return True if the shared instance has ever saved, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool HasSharedInstanceEverSaved() const;

	/**
	 * Checks if the shared save instance has ever loaded.
	 * @return True if the shared instance has ever loaded, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool HasSharedInstanceEverLoaded() const;

	/**
	 * Checks if the save manager is currently saving a save.
	 */
	UFUNCTION(BlueprintCallable)
	void StartTimePlayedTimer();

	/**
	 * Sets the SaveMasterID for the save manager.
	 * @param NewSaveMasterID - The new SaveMasterID to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetSaveMasterID(const FName NewSaveMasterID);

	/**
	 * Gets the SaveMasterID for the save manager.
	 * @return The current SaveMasterID.
	 */
	UFUNCTION(BlueprintPure)
	FName GetSaveMasterID() const;

	/**
	 * Saves the current game state.
	 * @param SaveName - The name of the save slot to save to.
	 * @param Instigator - The object that initiated the save.
	 * @param bIsSharedSave - Whether this is a shared save.
	 */
	void Save(const FString& SaveName, UObject* Instigator, const bool bIsSharedSave = false);

	/**
	 * Loads a save game from the specified slot.
	 * @param SaveName - The name of the save slot to load from.
	 * @param Instigator - The object that initiated the load.
	 * @param bIsSharedLoad - Whether this is a shared load.
	 */
	void Load(const FString& SaveName, UObject* Instigator, const bool bIsSharedLoad = false);

private:
	/**
	 * Called when a save operation is completed.
	 * @param SlotFullPathName - The full path name of the save slot.
	 * @param UserIndex - The user index for the save operation.
	 * @param bSuccess - Whether the save operation was successful.
	 */
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);

	/**
	 * Called when a shared save operation is completed.
	 * @param SlotFullPathName - The full path name of the shared save slot.
	 * @param UserIndex - The user index for the shared save operation.
	 * @param SaveGame - The save game instance that was saved.
	 */
	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);

	/**
	 * Called when a shared save operation is pending.
	 * @param Instigator - The object that initiated the shared save.
	 */
	UFUNCTION()
	void OnPendingSharedSaveEvent(UObject* Instigator);

	/**
	 * Called when a shared load operation is pending.
	 * @param Instigator - The object that initiated the shared load.
	 */
	UFUNCTION()
	void OnPendingSharedLoadEvent(UObject* Instigator);

	/**
	 * Updates the slot info with the new save slot name key.
	 * @param NewSaveSlotNameKey - The new save slot name key to update.
	 */
	void UpdateSlotInfo(const FName NewSaveSlotNameKey);

	/**
	 * Removes the slot info for the specified slot name key.
	 * @param SlotNameKey - The name key of the slot to remove.
	 */
	void RemoveSlotInfo(const FName SlotNameKey) const;

	/**
	 * Clears the current slot info item.
	 */
	void ClearSlotInfos() const;

	/**
	 * Loads the slot infos from the save system.
	 */
	void LoadSlotInfos();

	/**
	 * Creates a new save instance.
	 */
	void CreateNewSaveInstance();

	/**
	 * Creates a new shared save instance.
	 */
	void CreateSaveInstances();

	/**
	 * Updates the time played for the current save instance.
	 */
	void UpdateTimePlayed();

	/**
	 * Clears the time played for the current save instance.
	 */
	void ClearTimePlayed();

	/**
	 * Gets the elapsed time played since the last save.
	 * @return The elapsed time played in seconds.
	 */
	double GetElapsedTimePlayed();
};
