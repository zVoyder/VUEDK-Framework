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
	
	UFUNCTION(BlueprintCallable)
	FName GetUniqueSaveID() const;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);
	
	UFUNCTION()
	void PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION()
	void PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	UFUNCTION()
	void PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnPrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnSharedSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnSharedLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithNewSaveGameEvent(UDefaultSaveGame* SaveGame);

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithLoadedSharedSaveGameEvent(UDefaultSaveGame* SaveGame);

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginWithNewSharedSaveGameEvent(UDefaultSaveGame* SaveGame);

private:
	void CheckBeginWithLoad();
	
	void BeginWithLoadedSaveGame();

	void BeginWithNewSaveGame();

	void BeginWithLoadedSharedSaveGame();

	void BeginWithNewSharedSaveGame();
	
	bool Check() const;
};
