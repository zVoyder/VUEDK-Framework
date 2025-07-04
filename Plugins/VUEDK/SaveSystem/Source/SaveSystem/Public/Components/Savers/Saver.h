// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "SaveManager.h"
#include "Saver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBeginWithNewSaveGame,
	UDefaultSaveGame*, SaveGame
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBegingWithLoadedSharedSaveGame,
	UDefaultSaveGame*, SharedSaveGame
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBeginWithNewSharedSaveGame,
	UDefaultSaveGame*, SharedSaveGame
);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SAVESYSTEM_API USaver : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareSave OnPrepSave;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareLoad OnPrepLoad;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSaveGame OnSaveGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnLoadGame OnLoadGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareSharedSave OnPrepSharedSave;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPrepareSharedLoad OnPrepSharedLoad;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSharedSaveGame OnSharedSaveGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSharedLoadGame OnSharedLoadGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBeginWithNewSaveGame OnBeginWithNewSaveGame;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBegingWithLoadedSharedSaveGame OnBeginWithLoadedSharedSaveGame;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBeginWithNewSharedSaveGame OnBeginWithNewSharedSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay,
		meta = (ToolTip = "Useful when you want to delay the BeginWithLoad Events for the next tick in case you want to do some initialization before the events are called."))
	bool bDelayBeginWithLoadForNextTick = true;

protected:
	UPROPERTY()
	USaveManager* SaveManager;
	
public:
	USaver();
	
	/**
	 * Returns the unique save ID for this saver instance.
	 * @return The unique FName identifier for the save.
	 */
	UFUNCTION(BlueprintCallable)
	FName GetUniqueSaveID() const;
	
protected:
	/**
	 * Called when the component begins play. Used for initialization logic.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * Called when the component ends play. Used for cleanup logic.
	 * @param EndPlayReason - The reason the component is ending play.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/**
	 * Prepares the save operation with the given save game, slot info, and instigator.
	 * @param SaveGame - The save game object to prepare.
	 * @param SlotInfoItem - The slot info item associated with the save.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION()
	void PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);
	
	/**
	 * Prepares the load operation with the given save game and instigator.
	 * @param SaveGame - The save game object to prepare.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION()
	void PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Prepares a shared save operation with the given save game, slot info, and instigator.
	 * @param SaveGame - The save game object to prepare.
	 * @param SlotInfoItem - The slot info item associated with the save.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION()
	void PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	/**
	 * Prepares a shared load operation with the given save game and instigator.
	 * @param SaveGame - The save game object to prepare.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION()
	void PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called to prepare a save operation.
	 * @param SaveGame - The save game object to prepare.
	 * @param SlotInfoItem - The slot info item associated with the save.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	/**
	 * Event called to prepare a load operation.
	 * @param SaveGame - The save game object to prepare.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a save operation is completed.
	 * @param SlotName - The name of the save slot.
	 * @param UserIndex - The user index associated with the save.
	 * @param bSuccess - Indicates if the save operation was successful.
	 * @param SaveGame - The save game object that was saved.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a load operation is completed.
	 * @param SlotName - The name of the save slot.
	 * @param UserIndex - The user index associated with the load.
	 * @param LoadedData - The loaded save game data.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	/**
	 * Event called to prepare a shared save operation.
	 * @param SaveGame - The save game object to prepare.
	 * @param SlotInfoItem - The slot info item associated with the save.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	/**
	 * Event called to prepare a shared load operation.
	 * @param SaveGame - The save game object to prepare.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a shared save operation is completed.
	 * @param SlotName - The name of the save slot.
	 * @param UserIndex - The user index associated with the save.
	 * @param bSuccess - Indicates if the shared save operation was successful.
	 * @param SaveGame - The save game object that was saved.
	 * @param Instigator - The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSharedSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a shared load operation is completed.
	 * @param SlotName - The name of the save slot.
	 * @param UserIndex - The user index associated with the load.
	 * @param LoadedData - The loaded save game data.
	 * @param Instigator - The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSharedLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	/**
	 * Event called when the saver begins with a loaded save game.
	 * @param SaveGame - The save game object that was loaded.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithNewSaveGameEvent(UDefaultSaveGame* SaveGame);

	/**
	 * Event called when the saver begins with a loaded shared save game.
	 * @param SaveGame - The shared save game object that was loaded.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithLoadedSharedSaveGameEvent(UDefaultSaveGame* SaveGame);

	/**
	 * Event called when the saver begins with a new shared save game.
	 * @param SaveGame - The shared save game object that is new.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithNewSharedSaveGameEvent(UDefaultSaveGame* SaveGame);

private:
	/**
	 * Checks if the saver should begin with a loaded save game and triggers the appropriate logic.
	 */
	void CheckBeginWithLoad();

	/**
	 * Handles logic for beginning with a loaded save game.
	 */
	void BeginWithLoadedSaveGame();

	/**
	 * Handles logic for beginning with a new save game.
	 */
	void BeginWithNewSaveGame();

	/**
	 * Handles logic for beginning with a loaded shared save game.
	 */
	void BeginWithLoadedSharedSaveGame();

	/**
	 * Handles logic for beginning with a new shared save game.
	 */
	void BeginWithNewSharedSaveGame();

	/**
	 * Checks if the saver is in a valid state for operations.
	 * @return true if valid, false otherwise.
	 */
	bool Check() const;
};
