// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/Savers/Saver.h"
#include "Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"

USaver::USaver(): SaveManager(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName USaver::GetUniqueSaveID() const
{
	const FName MasterID = USSUtility::GetSaveMasterID();
	const FName OwnerName = GetOwner()->GetFName();
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	const FString UniqueID = OwnerName.ToString() + LevelName + MasterID.ToString();
	const int32 Hash = GetTypeHash(UniqueID);
	const FString Hex = FString::Printf(TEXT("%08X"), Hash);

	return FName(*Hex);
}

void USaver::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = USSUtility::GetSaveManager();

	if (!Check()) return;

	SaveManager->OnPrepareSave.AddDynamic(this, &USaver::PrepareSave);
	SaveManager->OnPrepareLoad.AddDynamic(this, &USaver::PrepareLoad);
	SaveManager->OnSaveGame.AddDynamic(this, &USaver::OnSaveCompletedEvent);
	SaveManager->OnLoadGame.AddDynamic(this, &USaver::OnLoadCompletedEvent);
	SaveManager->OnPrepareSharedSave.AddDynamic(this, &USaver::PrepareSharedSave);
	SaveManager->OnPrepareSharedLoad.AddDynamic(this, &USaver::PrepareSharedLoad);
	SaveManager->OnSharedSaveGame.AddDynamic(this, &USaver::OnSharedSaveCompletedEvent);
	SaveManager->OnSharedLoadGame.AddDynamic(this, &USaver::OnSharedLoadCompletedEvent);

	if (bDelayBeginWithLoadForNextTick)
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USaver::CheckBeginWithLoad);
	else
		CheckBeginWithLoad();
}

void USaver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check()) return;

	SaveManager->OnPrepareSave.RemoveDynamic(this, &USaver::PrepareSave);
	SaveManager->OnPrepareLoad.RemoveDynamic(this, &USaver::PrepareLoad);
	SaveManager->OnSaveGame.RemoveDynamic(this, &USaver::OnSaveCompletedEvent);
	SaveManager->OnLoadGame.RemoveDynamic(this, &USaver::OnLoadCompletedEvent);
	SaveManager->OnPrepareSharedSave.RemoveDynamic(this, &USaver::PrepareSharedSave);
	SaveManager->OnPrepareSharedLoad.RemoveDynamic(this, &USaver::PrepareSharedLoad);
	SaveManager->OnSharedSaveGame.RemoveDynamic(this, &USaver::OnSharedSaveCompletedEvent);
	SaveManager->OnSharedLoadGame.RemoveDynamic(this, &USaver::OnSharedLoadCompletedEvent);
}

void USaver::PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	OnPrepSave.Broadcast(SaveGame, SlotInfoItem, Instigator);
	OnPrepareSave(SaveGame, SlotInfoItem, Instigator);
}

void USaver::PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnPrepLoad.Broadcast(SaveGame, Instigator);
	OnPrepareLoad(SaveGame, Instigator);
}

void USaver::PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	OnPrepSharedSave.Broadcast(SaveGame, SlotInfoItem, Instigator);
	OnPrepareSharedSave(SaveGame, SlotInfoItem, Instigator);
}

void USaver::PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnPrepSharedLoad.Broadcast(SaveGame, Instigator);
	OnPrepareSharedLoad(SaveGame, Instigator);
}

void USaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameInstance, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
}

void USaver::OnPrepareLoad_Implementation(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
}

void USaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	OnLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData, Instigator);
}

void USaver::OnPrepareSharedSave_Implementation(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
}

void USaver::OnPrepareSharedLoad_Implementation(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSharedSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnSharedSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
}

void USaver::OnSharedLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	OnSharedLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData, Instigator);
}

void USaver::OnBeginWithNewSaveGameEvent_Implementation(UDefaultSaveGame* SaveGame)
{
}

void USaver::OnBeginWithLoadedSharedSaveGameEvent_Implementation(UDefaultSaveGame* SaveGame)
{
}

void USaver::OnBeginWithNewSharedSaveGameEvent_Implementation(UDefaultSaveGame* SaveGame)
{
}

void USaver::CheckBeginWithLoad()
{
	if (SaveManager->HasInstanceEverLoaded())
		BeginWithLoadedSaveGame();
	else
		BeginWithNewSaveGame();

	if (SaveManager->HasSharedInstanceEverLoaded())
		BeginWithLoadedSharedSaveGame();
	else
		BeginWithNewSharedSaveGame();
}

void USaver::BeginWithLoadedSaveGame()
{
	UDefaultSaveGame* SaveGame = SaveManager->GetSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithLoadedSaveGame(), SaveGame is not valid."));
		return;
	}

	OnLoadCompletedEvent(SaveGame->SlotNameKey.ToString(), 0, SaveGame, this);
}

void USaver::BeginWithNewSaveGame()
{
	UDefaultSaveGame* SaveGame = SaveManager->GetSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithNewSaveGame(), SaveGame is not valid."));
		return;
	}

	OnBeginWithNewSaveGameEvent(SaveGame);
	OnBeginWithNewSaveGame.Broadcast(SaveGame);
}

void USaver::BeginWithLoadedSharedSaveGame()
{
	UDefaultSaveGame* SaveGame = SaveManager->GetSharedSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithLoadedSharedSaveGame(), SharedSaveGame is not valid."));
		return;
	}

	OnBeginWithLoadedSharedSaveGameEvent(SaveGame);
	OnBeginWithLoadedSharedSaveGame.Broadcast(SaveGame);
}

void USaver::BeginWithNewSharedSaveGame()
{
	UDefaultSaveGame* SaveGame = SaveManager->GetSharedSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithNewSharedSaveGame(), SharedSaveGame is not valid."));
		return;
	}

	OnBeginWithNewSharedSaveGameEvent(SaveGame);
	OnBeginWithNewSharedSaveGame.Broadcast(SaveGame);
}

bool USaver::Check() const
{
	return SaveManager != nullptr;
}
