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

	UFUNCTION(BlueprintCallable)
	void Init(const FSaveManagerData SaveManagerData);

	UFUNCTION(BlueprintPure)
	UDefaultSaveGame* GetSaveGameInstance() const;

	UFUNCTION(BlueprintPure)
	UDefaultSaveGame* GetSharedSaveGameInstance() const;

	UFUNCTION(BlueprintCallable)
	bool DeleteSlot(const FName SlotName);

	UFUNCTION(BlueprintCallable)
	void DeleteAllSlots();

	UFUNCTION(BlueprintCallable)
	void StartNewSaveGame();

	UFUNCTION(BlueprintCallable)
	void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	UFUNCTION(Blueprintable)
	void SharedSave(UObject* Instigator);

	UFUNCTION(BlueprintCallable)
	void ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	UFUNCTION(BlueprintCallable)
	void LoadSelectedSlotAndSharedSlot(UObject* Instigator);

	UFUNCTION(BlueprintCallable)
	void LoadSelectedSlot(UObject* Instigator);

	UFUNCTION(BlueprintCallable)
	void LoadSharedSlot(UObject* Instigator);

	UFUNCTION(BlueprintPure)
	TArray<FSlotInfoData> GetSaveInfos() const;

	UFUNCTION(BlueprintPure)
	bool GetStatus(bool& OutIsLoading, bool& OutIsSaving) const;

	UFUNCTION(BlueprintPure)
	bool HasInstanceEverSaved() const;

	UFUNCTION(BlueprintPure)
	bool HasInstanceEverLoaded() const;

	UFUNCTION(BlueprintCallable)
	bool HasSharedInstanceEverSaved() const;

	UFUNCTION(BlueprintCallable)
	bool HasSharedInstanceEverLoaded() const;

	UFUNCTION(BlueprintCallable)
	void StartTimePlayedTimer();

	UFUNCTION(BlueprintCallable)
	void SetSaveMasterID(const FName NewSaveMasterID);

	UFUNCTION(BlueprintPure)
	FName GetSaveMasterID() const;

	void Save(const FString& SaveName, UObject* Instigator, const bool bIsSharedSave = false);

	void Load(const FString& SaveName, UObject* Instigator, const bool bIsSharedLoad = false);

private:
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);

	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);

	UFUNCTION()
	void OnPendingSharedSaveEvent(UObject* Instigator);

	UFUNCTION()
	void OnPendingSharedLoadEvent(UObject* Instigator);

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
