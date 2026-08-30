// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveManager.h"
#include "SaveSystem.h"
#include "Constants/SaveConstants.h"
#include "Data/Structs/SlotInfoData.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/SSSlotsUtility.h"
#include "Utility/SSUtility.h"

USaveManager::USaveManager() : SaveGameClass(UDefaultSaveGame::StaticClass()),
                               SharedSaveGameClass(UDefaultSaveGame::StaticClass()),
                               SlotInfoItemClass(USlotInfoItem::StaticClass()),
                               SharedSlotInfoItemClass(USlotInfoItem::StaticClass()),
                               ElapsedTimePlayed(0),
                               SlotSaveGameInstance(nullptr),
                               SharedSaveGameInstance(nullptr),
                               CurrentSlotInfos(nullptr),
                               CurrentSlotInfoItem(nullptr),
                               CurrentSharedSlotInfoItem(nullptr),
                               CurrentInstigator(nullptr),
                               bIsSharedSaving(false),
                               bIsSharedLoading(false),
                               bIsLoading(false),
                               bIsSaving(false),
                               bHasEverLoaded(false),
                               bHasEverSaved(false),
                               bHasEverSharedSaved(false),
                               bHasEverSharedLoaded(false),
                               PreviousSlotNameKey(NAME_None),
                               bSaveAsNewGame(false),
                               bIsNewSaveGame(false)
{
}

void USaveManager::Init(const FSaveManagerData SaveManagerData)
{
	if (!SaveManagerData.SaveGameClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Save Game Class is not valid."));
		return;
	}

	if (!SaveManagerData.SharedSaveGameClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Shared Save Game Class is not valid."));
		return;
	}

	if (!SaveManagerData.SlotInfoItemClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Save Info Class is not valid."));
		return;
	}

	SaveGameClass = SaveManagerData.SaveGameClass;
	SharedSaveGameClass = SaveManagerData.SharedSaveGameClass;
	SlotInfoItemClass = SaveManagerData.SlotInfoItemClass;
	SharedSlotInfoItemClass = SaveManagerData.SharedSlotInfoItemClass;

	CreateSaveInstances();
	LoadSlotInfos(); // Load slot infos before initializing other utilities

	USSUtility::Init(this);
	USSSlotsUtility::Init(this);

	UE_LOG(LogSaveSystem, Display, TEXT("Save System Initialized"));
}

UDefaultSaveGame* USaveManager::GetSaveGameInstance() const
{
	return SlotSaveGameInstance;
}

UDefaultSaveGame* USaveManager::GetSharedSaveGameInstance() const
{
	return SharedSaveGameInstance;
}

bool USaveManager::DeleteSlot(const FName SlotName) const
{
	if (!UGameplayStatics::DeleteGameInSlot(SAVES_DIRECTORY + SlotName.ToString(), 0))
		return false;

	RemoveSlotInfo(SlotName);

	if (SlotName == USSSlotsUtility::GetSelectedSlotName())
	{
		USSSlotsUtility::ClearSelectedSlotName();
		USSSlotsUtility::TrySelectMostRecentSaveGame(); // Select the most recent slot if is the same slot
	}

	return true;
}

void USaveManager::DeleteAllSlots()
{
	CreateSaveInstances();
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (FileManager.DeleteDirectory(*SaveDirectory, false, true))
		ClearSlotInfos();
}

void USaveManager::StartNewSaveGame()
{
	if (bIsLoading || bIsSaving)
		return;

	CreateNewSaveInstance();
	bSaveAsNewGame = true;
	OnNewSaveGame.Broadcast();
	ManualSave(this, false, false);
	bIsNewSaveGame = true;
}

void USaveManager::ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot, const bool bAsync)
{
	bIsNewSaveGame = false;
	if (bOverrideCurrentSlot && USSSlotsUtility::DoesAnySlotFileExist())
	{
		Save(USSSlotsUtility::GetSelectedSlotName(), Instigator, false, bAsync);
		return;
	}

	const int32 SaveSlots = USSSlotsUtility::GetTotalManualSaveSlots();
	const FString NextManualSlotName = SAVE_SLOT_NAME + FString::FromInt(SaveSlots);
	Save(NextManualSlotName, Instigator, false, bAsync);
}

void USaveManager::SharedSave(UObject* Instigator, const bool bAsync)
{
	Save(SHARED_SAVE_FILE_NAME, Instigator, true, bAsync);
}

void USaveManager::ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot, const bool bAsync)
{
	OnPendingSharedSave.AddUniqueDynamic(this, &USaveManager::OnPendingSharedSaveEvent);
	ManualSave(Instigator, bOverrideCurrentSlot, bAsync);
}

void USaveManager::LoadSelectedSlotAndSharedSlot(UObject* Instigator, const bool bAsync)
{
	if (!USSSlotsUtility::IsSelectedSlotValid())
	{
		LoadSharedSlot(Instigator);
		return;
	}

	OnPendingSharedLoad.AddUniqueDynamic(this, &USaveManager::OnPendingSharedLoadEvent);
	LoadSelectedSlot(Instigator, bAsync);
}

void USaveManager::LoadSelectedSlot(UObject* Instigator, const bool bAsync)
{
	if (!USSSlotsUtility::IsSelectedSlotValid())
		return;

	Load(USSSlotsUtility::GetSelectedSlotName(), Instigator, false, bAsync);
}

void USaveManager::LoadSharedSlot(UObject* Instigator, const bool bAsync)
{
	if (!USSSlotsUtility::DoesSharedSlotFileExist())
		return;

	Load(SHARED_SAVE_FILE_NAME, Instigator, true, bAsync);
}

TArray<FSlotInfoData> USaveManager::GetSaveInfos() const
{
	TArray<FSlotInfoData> Infos;
	if (!CurrentSlotInfos)
		return Infos;

	CurrentSlotInfos->SlotInfos.GenerateValueArray(Infos);

	// Sort in descending order, so the most recent save is the first one
	Infos.Sort();
	Algo::Reverse(Infos);
	return Infos;
}

bool USaveManager::GetStatus(bool& OutIsLoading, bool& OutIsSaving) const
{
	OutIsLoading = bIsLoading;
	OutIsSaving = bIsSaving;
	return bIsLoading || bIsSaving;
}

bool USaveManager::IsNewSaveGame() const
{
	return bIsNewSaveGame;
}

bool USaveManager::HasInstanceEverSaved() const
{
	return bHasEverSaved;
}

bool USaveManager::HasInstanceEverLoaded() const
{
	return bHasEverLoaded;
}

bool USaveManager::HasSharedInstanceEverSaved() const
{
	return bHasEverSharedSaved;
}

bool USaveManager::HasSharedInstanceEverLoaded() const
{
	return bHasEverSharedLoaded;
}

void USaveManager::StartTimePlayedTimer()
{
	const UWorld* World = GetWorld();
	if (!World) return;

	ClearTimePlayed();
	World->GetTimerManager().ClearTimer(TimePlayedTimerHandle);
	World->GetTimerManager().SetTimer(TimePlayedTimerHandle, this, &USaveManager::UpdateTimePlayed,
	                                  TIME_PLAYED_TIMER_RATE, true);
}

void USaveManager::Save(const FString& SaveName, UObject* Instigator, const bool bIsSharedSave, const bool bAsync)
{
	UDefaultSaveGame*& SaveGameInstance = bIsSharedSave ? SharedSaveGameInstance : SlotSaveGameInstance;

	if (!USSSlotsUtility::IsSlotNameValid(SaveName) ||
		!IsValid(SaveGameInstance) ||
		bIsLoading || bIsSaving)
		return;

	if (!Instigator)
		Instigator = this;

	bIsSharedSaving = bIsSharedSave;
	CurrentInstigator = Instigator;
	PreviousSlotNameKey = SaveGameInstance->SlotNameKey;
	const FName SlotFName = FName(*SaveName);
	SaveGameInstance->SetSlotNameKey(SlotFName);

	// Notify the game that it's going to save, so all the saver objects can push their data to the save game object
	if (bIsSharedSaving)
	{
		CurrentSharedSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SharedSlotInfoItemClass));
		OnPrepareSharedSave.Broadcast(SaveGameInstance, CurrentSharedSlotInfoItem, Instigator);
	}
	else
	{
		CurrentSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));
		OnPrepareSave.Broadcast(SaveGameInstance, CurrentSlotInfoItem, Instigator);
	}

	bIsSaving = true;

	if (!bAsync)
	{
		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVES_DIRECTORY + SaveName, 0);
		OnSaveCompletedSynchronous(SAVES_DIRECTORY + SaveName, 0, bSuccess);
		return;
	}
	
	FAsyncSaveGameToSlotDelegate AsyncSaveDelegate;
	AsyncSaveDelegate.BindUObject(this, &USaveManager::OnSaveCompletedAsync);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SAVES_DIRECTORY + SaveName, 0, AsyncSaveDelegate);
}

void USaveManager::Load(const FString& SaveName, UObject* Instigator, const bool bIsSharedLoad, const bool bAsync)
{
	UDefaultSaveGame*& SaveGameInstance = bIsSharedLoad ? SharedSaveGameInstance : SlotSaveGameInstance;

	if (!USSSlotsUtility::IsSlotNameValid(SaveName) ||
		!IsValid(SaveGameInstance) ||
		bIsLoading || bIsSaving)
		return;

	if (!Instigator)
		Instigator = this;

	bIsSharedLoading = bIsSharedLoad;
	CurrentInstigator = Instigator;

	if (bIsSharedLoading)
		OnPrepareSharedLoad.Broadcast(SaveGameInstance, Instigator);
	else
		OnPrepareLoad.Broadcast(SaveGameInstance, Instigator);

	bIsLoading = true;

	if (!bAsync)
	{
		USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SAVES_DIRECTORY + SaveName, 0);
		OnLoadCompletedSynchronous(SAVES_DIRECTORY + SaveName, 0, LoadedGame);
		return;
	}
	
	FAsyncLoadGameFromSlotDelegate AsyncLoadDelegate;
	AsyncLoadDelegate.BindUObject(this, &USaveManager::OnLoadCompletedAsync);
	UGameplayStatics::AsyncLoadGameFromSlot(SAVES_DIRECTORY + SaveName, 0, AsyncLoadDelegate);
}

void USaveManager::OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess)
{
	if (!bSuccess)
		UE_LOG(LogSaveSystem, Error, TEXT("Failed to save game."))
	else
	{
		UDefaultSaveGame*& SaveGameInstance = bIsSharedSaving ? SharedSaveGameInstance : SlotSaveGameInstance;
		const FString NewSaveSlotName = SaveGameInstance->SlotNameKey.ToString();
		UpdateSlotInfo(FName(*NewSaveSlotName));
		USSSlotsUtility::TrySelectSaveGameSlot(NewSaveSlotName);

		if (bIsSharedSaving)
		{
			bHasEverSharedSaved = true;
			OnSharedSaveGame.Broadcast(NewSaveSlotName, UserIndex, bSuccess, SaveGameInstance, CurrentInstigator);
		}
		else
		{
			bHasEverSaved = true;
			OnSaveGame.Broadcast(NewSaveSlotName, UserIndex, bSuccess, SaveGameInstance, CurrentInstigator);
		}

		UE_LOG(LogSaveSystem, Display, TEXT("Game saved successfully."));
	}

	bIsSaving = false;
	bIsSharedSaving = false;
	bSaveAsNewGame = false;
}

void USaveManager::OnSaveCompletedAsync(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess)
{
	OnSaveCompleted(SlotFullPathName, UserIndex, bSuccess);
	OnPendingSharedSave.Broadcast(CurrentInstigator, true);
}

void USaveManager::OnSaveCompletedSynchronous(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess)
{
	OnSaveCompleted(SlotFullPathName, UserIndex, bSuccess);
	OnPendingSharedSave.Broadcast(CurrentInstigator, false);
}

void USaveManager::OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame)
{
	const bool bSuccess = SaveGame != nullptr;
	bIsLoading = false;

	if (!bSuccess)
		UE_LOG(LogSaveSystem, Error, TEXT("Failed to load game."))
	else
	{
		UDefaultSaveGame*& SaveGameInstance = bIsSharedLoading ? SharedSaveGameInstance : SlotSaveGameInstance;

		if (SaveGameInstance = Cast<UDefaultSaveGame>(SaveGame); !IsValid(SaveGameInstance))
			UE_LOG(LogSaveSystem, Error, TEXT("Failed Cast: Loaded game is not of type %s"), *SaveGameClass->GetName())
		else
		{
			const FString SlotInfoName = SaveGameInstance->SlotNameKey.ToString();

			if (bIsSharedLoading)
			{
				bHasEverSharedLoaded = true;
				OnSharedLoadGame.Broadcast(SlotInfoName, UserIndex, SaveGameInstance, CurrentInstigator);
			}
			else
			{
				bHasEverLoaded = true;
				OnLoadGame.Broadcast(SlotInfoName, UserIndex, SaveGameInstance, CurrentInstigator);
			}

			UE_LOG(LogSaveSystem, Display, TEXT("Game loaded successfully."));
		}
	}

	bIsSharedLoading = false;
}

void USaveManager::OnLoadCompletedAsync(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame)
{
	OnLoadCompleted(SlotFullPathName, UserIndex, SaveGame);
	OnPendingSharedLoad.Broadcast(CurrentInstigator, true);
}

void USaveManager::OnLoadCompletedSynchronous(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame)
{
	OnLoadCompleted(SlotFullPathName, UserIndex, SaveGame);
	OnPendingSharedLoad.Broadcast(CurrentInstigator, false);
}

void USaveManager::OnPendingSharedSaveEvent(UObject* Instigator, const bool bAsync)
{
	OnPendingSharedSave.RemoveDynamic(this, &USaveManager::OnPendingSharedSaveEvent);
	SharedSave(Instigator, bAsync);
}

void USaveManager::OnPendingSharedLoadEvent(UObject* Instigator, const bool bAsync)
{
	OnPendingSharedLoad.RemoveDynamic(this, &USaveManager::OnPendingSharedLoadEvent);
	USSUtility::LoadSharedSlot(Instigator);
}

void USaveManager::UpdateSlotInfo(const FName NewSaveSlotNameKey)
{
	if (!CurrentSlotInfos)
		return;

	float CurrentPlayedTime = 0.f;
	USlotInfoItem*& SlotInfoItem = bIsSharedSaving ? CurrentSharedSlotInfoItem : CurrentSlotInfoItem; // Determine if it's a shared slot or not

	if (!bSaveAsNewGame && CurrentSlotInfos->SlotInfos.Contains(PreviousSlotNameKey))
		CurrentPlayedTime = CurrentSlotInfos->SlotInfos[PreviousSlotNameKey].TimePlayed;

	if (!CurrentSlotInfos->SlotInfos.Contains(NewSaveSlotNameKey))
	{
		FSlotInfoData NewSlotInfo = FSlotInfoData(NewSaveSlotNameKey.ToString());
		NewSlotInfo.TimePlayed = CurrentPlayedTime;
		NewSlotInfo.SlotInfoItem = SlotInfoItem;
		CurrentSlotInfos->SlotInfos.Add(NewSaveSlotNameKey, NewSlotInfo);
	}

	CurrentSlotInfos->SlotInfos[NewSaveSlotNameKey].LastSaveDate = FDateTime::Now();
	CurrentSlotInfos->SlotInfos[NewSaveSlotNameKey].TimePlayed += GetElapsedTimePlayed();

	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);
	UGameplayStatics::SaveGameToSlot(SlotInfoItem, META_DIRECTORY + NewSaveSlotNameKey.ToString(), 0);
}

void USaveManager::RemoveSlotInfo(const FName SlotNameKey) const
{
	CurrentSlotInfos->SlotInfos.Remove(SlotNameKey);
	UGameplayStatics::DeleteGameInSlot(META_DIRECTORY + SlotNameKey.ToString(), 0);
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);
}

void USaveManager::ClearSlotInfos() const
{
	CurrentSlotInfos->SlotInfos.Empty();
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);

	const FString FullPath = META_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*META_DIRECTORY_FULLPATH, false, true);
}

void USaveManager::LoadSlotInfos()
{
	if (USlotInfos* Infos = Cast<USlotInfos>(UGameplayStatics::LoadGameFromSlot(META_INFOS_PATH, 0)))
	{
		CurrentSlotInfos = Infos;

		for (auto& SlotInfo : CurrentSlotInfos->SlotInfos)
		{
			USlotInfoItem* LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::LoadGameFromSlot(META_DIRECTORY + SlotInfo.Value.SlotInfoName, 0));
			if (!LoadedInfoItem)
			{
				TSubclassOf<USlotInfoItem> InfoItemClass = USSSlotsUtility::IsSharedSlot(SlotInfo.Value) ? SharedSlotInfoItemClass : SlotInfoItemClass;
				LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(InfoItemClass));
			}
			SlotInfo.Value.SlotInfoItem = LoadedInfoItem;
		}
	}
}

void USaveManager::CreateNewSaveInstance()
{
	SlotSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	SharedSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SharedSaveGameClass));
	bHasEverLoaded = false;
	bHasEverSaved = false;
}

void USaveManager::CreateSaveInstances()
{
	CreateNewSaveInstance();
	CurrentSlotInfos = Cast<USlotInfos>(UGameplayStatics::CreateSaveGameObject(USlotInfos::StaticClass()));
	CurrentSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));
	CurrentSharedSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SharedSlotInfoItemClass));
}

void USaveManager::UpdateTimePlayed()
{
	ElapsedTimePlayed += TIME_PLAYED_TIMER_RATE;
}

void USaveManager::ClearTimePlayed()
{
	ElapsedTimePlayed = 0.f;
}

double USaveManager::GetElapsedTimePlayed()
{
	const double TimePlayed = ElapsedTimePlayed;
	ClearTimePlayed();
	return TimePlayed;
}
