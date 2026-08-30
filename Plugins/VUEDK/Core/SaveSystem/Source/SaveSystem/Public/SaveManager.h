// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveGame/DefaultSaveGame.h"
#include "Data/SaveGame/SlotInfoItem.h"
#include "Data/SaveGame/SlotInfos.h"
#include "Data/Structs/SaveManagerData.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPendingSharedSave,
	UObject*, Instigator,
	bool, bAsync
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPendingSharedLoad,
	UObject*, Instigator,
	bool, bAsync
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnNewSaveGame
);

UCLASS(NotBlueprintable, BlueprintType)
class SAVESYSTEM_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
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
	bool bIsNewSaveGame;
	FTimerHandle TimePlayedTimerHandle;

	FOnPendingSharedSave OnPendingSharedSave;
	FOnPendingSharedLoad OnPendingSharedLoad;

public:
	USaveManager();

	/**
	 * Initializes the SaveManager with the provided SaveManagerData.
	 * @param SaveManagerData The data structure containing configuration for the SaveManager.
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
	 * @param SlotName The name of the slot to retrieve the info for.
	 * @return The slot info item for the specified slot name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool DeleteSlot(const FName SlotName) const;

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
	 * @param Instigator The object that initiated the save.
	 * @param bOverrideCurrentSlot Whether to override the current save slot.
	 * @param bAsync Whether to save asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false, const bool bAsync = true);

	/**
	 * Saves the current game state to a shared slot.
	 * @param Instigator The object that initiated the shared save.
	 * @param bAsync Whether to save asynchronously.
	 */
	UFUNCTION(Blueprintable)
	void SharedSave(UObject* Instigator, const bool bAsync = true);

	/**
	 * Manually saves the current game state and also saves to a shared slot.
	 * @param Instigator The object that initiated the save.
	 * @param bOverrideCurrentSlot Whether to override the current save slot.
	 * @param bAsync Whether to save asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot = false, const bool bAsync = true);

	/**
	 * Selects a save slot and saves the current game state to it.
	 * @param Instigator The object that initiated the save.
	 * @param bAsync Whether to load asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSelectedSlotAndSharedSlot(UObject* Instigator, const bool bAsync = true);

	/**
	 * Loads the selected save slot.
	 * @param Instigator The object that initiated the load.<<
	 * @param bAsync Whether to load asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSelectedSlot(UObject* Instigator, const bool bAsync = true);

	/**
	 * Loads a shared save slot.
	 * @param Instigator The object that initiated the load.
	 * @param bAsync Whether to load asynchronously.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSharedSlot(UObject* Instigator, const bool bAsync = true);

	/**
	 * Loads the selected save slot and a shared save slot.
	 * @return True if the load was successful, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FSlotInfoData> GetSaveInfos() const;

	/**
	 * Gets the current slot info item.
	 * @param OutIsLoading Whether the save manager is currently loading.
	 * @param OutIsSaving Whether the save manager is currently saving.
	 * @return True if the status was retrieved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool GetStatus(bool& OutIsLoading, bool& OutIsSaving) const;

	/**
	 * Checks if the save manager has started a new save game.
	 * @return True if it is a new save game, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsNewSaveGame() const;

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
	 * Saves the current game state.
	 * @param SaveName The name of the save slot to save to.
	 * @param Instigator The object that initiated the save.
	 * @param bIsSharedSave Whether this is a shared save.
	 * @param bAsync Whether to save asynchronously.
	 */
	void Save(const FString& SaveName, UObject* Instigator, const bool bIsSharedSave = false, const bool bAsync = true);

	/**
	 * Loads a save game from the specified slot.
	 * @param SaveName The name of the save slot to load from.
	 * @param Instigator The object that initiated the load.
	 * @param bIsSharedLoad Whether this is a shared load.
	 * @param bAsync Whether to load asynchronously.
	 */
	void Load(const FString& SaveName, UObject* Instigator, const bool bIsSharedLoad = false, const bool bAsync = true);

private:
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);

	void OnSaveCompletedAsync(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);

	void OnSaveCompletedSynchronous(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);

	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);

	void OnLoadCompletedAsync(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);

	void OnLoadCompletedSynchronous(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);

	UFUNCTION()
	void OnPendingSharedSaveEvent(UObject* Instigator, const bool bAsync);

	UFUNCTION()
	void OnPendingSharedLoadEvent(UObject* Instigator, const bool bAsync);

	void UpdateSlotInfo(const FName NewSaveSlotNameKey);

	void RemoveSlotInfo(const FName SlotNameKey) const;

	void ClearSlotInfos() const;

	void LoadSlotInfos();

	void CreateNewSaveInstance();

	void CreateSaveInstances();

	void UpdateTimePlayed();

	void ClearTimePlayed();

	double GetElapsedTimePlayed();
};
