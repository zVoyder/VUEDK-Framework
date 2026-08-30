// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SaveGame/DefaultSaveGame.h"
#include "SaveManager.h"
#include "Behaviours/SaveBehaviour.h"
#include "Data/SaveData/SaveDataBase.h"
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
	FOnBeginWithNewSharedSaveGame OnBeginWithNewSharedSaveGame;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TSet<USaveBehaviour*> SaveBehaviours;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TSet<USaveBehaviour*> SharedSaveBehaviours;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay,
		meta = (ToolTip = "If true, OnPrepSave will attempt to serialize the owner actor, and OnLoadGame will attempt to deserialize it. Keep in mind this is not optimal; it is recommended to package your data in a SaveData object instead."))
	bool bSerializeOwner = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay,
		meta = (ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "Useful when you want to delay the LoadCompleted events at begin in case you want to do some initialization before the events are called."))
	float BeginWithLoadDelay = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay,
		meta = (ToolTip = "If true, the UniqueSaveID will be automatically generated from the owner's ActorGuid. This is useful to ensure uniqueness without manual setup, but it requires the owner to be an actor with a valid ActorGuid. If false, you must manually assign a UniqueSaveID for this saver instance."))
	bool bUseActorGuidAsSaveID = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, meta = (EditCondition = "!bUseActorGuidAsSaveID",
		ToolTip = "The unique identifier for this saver instance."))
	FGuid UniqueSaveID;

protected:
	UPROPERTY()
	USaveManager* SaveManager;

private:
	FTimerHandle BeginWithLoadTimerHandle;

public:
	USaver();

	/**
	 * Returns the unique save Id for this saver instance.
	 * @return The unique FGuid identifier for the save.
	 */
	UFUNCTION(BlueprintPure)
	FGuid GetUniqueSaveID() const;

	/**
	 * Combines the provided SaveDataID with the unique save Id of this saver instance.
	 * @param SaveDataID The sub-identifier for the SaveData.
	 * @return A composite FName that uniquely identifies the SaveData in the context of this saver.
	 */
	UFUNCTION(BlueprintPure)
	FName MakeCompositeSaveID(const FName SaveDataID) const;

	/**
	 * Pushes some SaveData to the SaveGame instance (calling function TrySerializeSaveDataObjectInSaveGame internally).
	 * @param SaveData The SaveData object to push data to.
	 * @param bIsSharedSave Whether to push to the shared save or the regular save.
	 * @return True if the operation was successful, False otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool PushDataToSaveGame(USaveDataBase* SaveData, const bool bIsSharedSave = false) const;

	/**
	 * Pulls some SaveData from the SaveGame instance (calling function TryDeserializeSaveDataObjectFromSaveGame internally).
	 * @param SaveData The SaveData object to populate with data from the save.
	 * @param bIsSharedSave Whether to pull from the shared save or the regular save.
	 * @return True if the operation was successful and data was found, False otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool PullDataFromSaveGame(USaveDataBase* SaveData, const bool bIsSharedSave = false) const;

	UFUNCTION(BlueprintPure)
	TArray<USaveBehaviour*> GetAllSaveBehaviours() const;

protected:
#if WITH_EDITOR
	virtual void PostInitProperties() override;

	virtual void PostLoad() override;
#endif
	
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	/**
	 * Event called to prepare a load operation.
	 * @param SaveGame The save game object to prepare.
	 * @param Instigator The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a save operation is completed.
	 * @param SlotName The name of the save slot.
	 * @param UserIndex The user index associated with the save.
	 * @param bSuccess Indicates if the save operation was successful.
	 * @param SaveGame The save game object that was saved.
	 * @param Instigator The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a load operation is completed.
	 * @param SlotName The name of the save slot.
	 * @param UserIndex The user index associated with the load.
	 * @param LoadedData The loaded save game data.
	 * @param Instigator The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	/**
	 * Event called to prepare a shared save operation.
	 * @param SaveGame The save game object to prepare.
	 * @param SlotInfoItem The slot info item associated with the save.
	 * @param Instigator The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	/**
	 * Event called to prepare a shared load operation.
	 * @param SaveGame The save game object to prepare.
	 * @param Instigator The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a shared save operation is completed.
	 * @param SlotName The name of the save slot.
	 * @param UserIndex The user index associated with the save.
	 * @param bSuccess Indicates if the shared save operation was successful.
	 * @param SaveGame The save game object that was saved.
	 * @param Instigator The object that initiated the save.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSharedSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	/**
	 * Event called when a shared load operation is completed.
	 * @param SlotName The name of the save slot.
	 * @param UserIndex The user index associated with the load.
	 * @param LoadedData The loaded save game data.
	 * @param Instigator The object that initiated the load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSharedLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	/**
	 * Event called when the saver begins with a loaded save game.
	 * @param SaveGame The save game object that was loaded.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithNewSaveGameEvent(UDefaultSaveGame* SaveGame);
	
	/**
	 * Event called when the saver begins with a new shared save game.
	 * @param SaveGame The shared save game object that is new.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithNewSharedSaveGameEvent(UDefaultSaveGame* SaveGame);

private:
	UFUNCTION()
	void PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);
	
	UFUNCTION()
	void PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION()
	void CompleteSave(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION()
	void CompleteLoad(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	UFUNCTION()
	void PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);
	
	UFUNCTION()
	void PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION()
	void CompleteSharedSave(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION()
	void CompleteSharedLoad(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);
	
	void SerializeOwner() const;

	void DeserializeOwner() const;

	void StartBeginWithLoad();
	
	void ExecuteBeginWithLoad();
	
	void BeginWithLoadedSaveGame();
	
	void BeginWithNewSaveGame();
	
	void BeginWithLoadedSharedSaveGame();
	
	void BeginWithNewSharedSaveGame();

	void SaveAllBehaviours();

	void SaveAllSharedBehaviours();

	void LoadAllBehaviours();

	void LoadAllSharedBehaviours();

	void InitSaveBehaviours();

	void EndPlaySaveBehaviours(const EEndPlayReason::Type EndPlayReason) const;
	
	bool Check() const;
};
