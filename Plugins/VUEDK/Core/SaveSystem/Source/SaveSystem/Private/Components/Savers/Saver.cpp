// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/Savers/Saver.h"
#include "SaveSystem.h"
#include "Constants/SaveConstants.h"
#include "Utility/SSUtility.h"
#include "Utility/SSSerializationUtility.h"

USaver::USaver() : SaveManager(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	UActorComponent::SetAutoActivate(true);
}

FGuid USaver::GetUniqueSaveID() const
{
	return UniqueSaveID;
}

FName USaver::MakeCompositeSaveID(const FName SaveDataID) const
{
	return FName(*FString::Printf(TEXT("%s_%s"), *GetUniqueSaveID().ToString(), *SaveDataID.ToString()));
}

bool USaver::PushDataToSaveGame(USaveDataBase* SaveData, const bool bIsSharedSave) const
{
	if (!IsValid(SaveData))
		return false;

	const FName SaveDataID = FName(UniqueSaveID.ToString());
	SaveData->SetSaveDataID(SaveDataID);
	return USSSerializationUtility::TrySerializeSaveDataObjectInSaveGame(SaveData, bIsSharedSave);
}

bool USaver::PullDataFromSaveGame(USaveDataBase* SaveData, const bool bIsSharedSave) const
{
	if (!IsValid(SaveData))
		return false;

	const FName SaveDataID = FName(UniqueSaveID.ToString());
	SaveData->SetSaveDataID(SaveDataID);
	return USSSerializationUtility::TryDeserializeSaveDataObjectFromSaveGame(SaveData, bIsSharedSave);
}

TArray<USaveBehaviour*> USaver::GetAllSaveBehaviours() const
{
	TArray<USaveBehaviour*> AllBehaviours = SaveBehaviours.Array();
	AllBehaviours.Append(SharedSaveBehaviours.Array());
	return AllBehaviours;
}

#if WITH_EDITOR
void USaver::PostInitProperties()
{
	Super::PostInitProperties();

	if (UniqueSaveID.IsValid())
		return;

	UniqueSaveID = FGuid::NewGuid();
}

void USaver::PostLoad()
{
	Super::PostLoad();

	if (!bUseActorGuidAsSaveID)
		return;
	
	if (!UniqueSaveID.IsValid())
	{
		UniqueSaveID = FGuid::NewGuid();
		return;
	}

	const AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		if (!Owner->HasAnyFlags(RF_Transient))
			UniqueSaveID = Owner->GetActorGuid();
	}
}
#endif

void USaver::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = USSUtility::GetSaveManager();

	if (!Check())
		return;

	InitSaveBehaviours();
	SaveManager->OnPrepareSave.AddDynamic(this, &USaver::PrepareSave);
	SaveManager->OnPrepareLoad.AddDynamic(this, &USaver::PrepareLoad);
	SaveManager->OnSaveGame.AddDynamic(this, &USaver::CompleteSave);
	SaveManager->OnLoadGame.AddDynamic(this, &USaver::CompleteLoad);
	SaveManager->OnPrepareSharedSave.AddDynamic(this, &USaver::PrepareSharedSave);
	SaveManager->OnPrepareSharedLoad.AddDynamic(this, &USaver::PrepareSharedLoad);
	SaveManager->OnSharedSaveGame.AddDynamic(this, &USaver::CompleteSharedSave);
	SaveManager->OnSharedLoadGame.AddDynamic(this, &USaver::CompleteSharedLoad);
	StartBeginWithLoad();
}

void USaver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check())
		return;

	EndPlaySaveBehaviours(EndPlayReason);
	SaveManager->OnPrepareSave.RemoveDynamic(this, &USaver::PrepareSave);
	SaveManager->OnPrepareLoad.RemoveDynamic(this, &USaver::PrepareLoad);
	SaveManager->OnSaveGame.RemoveDynamic(this, &USaver::CompleteSave);
	SaveManager->OnLoadGame.RemoveDynamic(this, &USaver::CompleteLoad);
	SaveManager->OnPrepareSharedSave.RemoveDynamic(this, &USaver::PrepareSharedSave);
	SaveManager->OnPrepareSharedLoad.RemoveDynamic(this, &USaver::PrepareSharedLoad);
	SaveManager->OnSharedSaveGame.RemoveDynamic(this, &USaver::CompleteSharedSave);
	SaveManager->OnSharedLoadGame.RemoveDynamic(this, &USaver::CompleteSharedLoad);
}

void USaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameInstance, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
}

void USaver::OnPrepareLoad_Implementation(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
}

void USaver::OnPrepareSharedSave_Implementation(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
}

void USaver::OnPrepareSharedLoad_Implementation(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSharedSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSharedLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
}

void USaver::OnBeginWithNewSaveGameEvent_Implementation(UDefaultSaveGame* SaveGame)
{
}

void USaver::OnBeginWithNewSharedSaveGameEvent_Implementation(UDefaultSaveGame* SaveGame)
{
}

void USaver::PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	if (!IsActive())
		return;

	OnPrepSave.Broadcast(SaveGame, SlotInfoItem, Instigator);
	OnPrepareSave(SaveGame, SlotInfoItem, Instigator);

	if (bSerializeOwner)
		SerializeOwner();

	SaveAllBehaviours();
}

void USaver::PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	if (!IsActive())
		return;

	OnPrepLoad.Broadcast(SaveGame, Instigator);
	OnPrepareLoad(SaveGame, Instigator);
}

void USaver::CompleteSave(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	if (!IsActive())
		return;

	OnSaveCompletedEvent(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
	OnSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
}

void USaver::CompleteLoad(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	if (!IsActive())
		return;

	if (bSerializeOwner)
		DeserializeOwner();

	LoadAllBehaviours();
	OnLoadCompletedEvent(SlotName, UserIndex, LoadedData, Instigator);
	OnLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData, Instigator);
}

void USaver::PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	if (!IsActive())
		return;

	OnPrepSharedSave.Broadcast(SaveGame, SlotInfoItem, Instigator);
	OnPrepareSharedSave(SaveGame, SlotInfoItem, Instigator);
	SaveAllSharedBehaviours();
}

void USaver::PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	if (!IsActive())
		return;

	OnPrepSharedLoad.Broadcast(SaveGame, Instigator);
	OnPrepareSharedLoad(SaveGame, Instigator);
}

void USaver::CompleteSharedSave(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	if (!IsActive())
		return;

	OnSharedSaveCompletedEvent(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
	OnSharedSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
}

void USaver::CompleteSharedLoad(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	if (!IsActive())
		return;

	LoadAllSharedBehaviours();
	OnSharedLoadCompletedEvent(SlotName, UserIndex, LoadedData, Instigator);
	OnSharedLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData, Instigator);
}

void USaver::SerializeOwner() const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
		return;

	const FName FullSaveID = MakeCompositeSaveID("SerializeOwner");
	USSSerializationUtility::TrySerializeObjectInSaveGame(Owner, FullSaveID);
}

void USaver::DeserializeOwner() const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
		return;

	const FName FullSaveID = MakeCompositeSaveID("SerializeOwner");
	USSSerializationUtility::TryDeserializeObjectFromSaveGame(Owner, FullSaveID);
}

void USaver::StartBeginWithLoad()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	if (BeginWithLoadDelay <= 0.f)
	{
		World->GetTimerManager().SetTimerForNextTick(
			this,
			&USaver::ExecuteBeginWithLoad
		);
		return;
	}

	World->GetTimerManager().SetTimer(
		BeginWithLoadTimerHandle,
		this,
		&USaver::ExecuteBeginWithLoad,
		BeginWithLoadDelay,
		false
	);
}

void USaver::ExecuteBeginWithLoad()
{
	const bool bHasEverLoaded = SaveManager->HasInstanceEverLoaded();
	const bool bHasEverSharedLoaded = SaveManager->HasSharedInstanceEverLoaded();

	if (bHasEverLoaded)
		BeginWithLoadedSaveGame();
	else
		BeginWithNewSaveGame();

	if (bHasEverSharedLoaded)
		BeginWithLoadedSharedSaveGame();
	else
		BeginWithNewSharedSaveGame();
}

void USaver::BeginWithLoadedSaveGame()
{
	if (!IsActive())
		return;

	UDefaultSaveGame* SaveGame = SaveManager->GetSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithLoadedSaveGame(), SaveGame is not valid."));
		return;
	}

	CompleteLoad(SaveGame->SlotNameKey.ToString(), 0, SaveGame, this);
}

void USaver::BeginWithNewSaveGame()
{
	if (!IsActive())
		return;

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
	if (!IsActive())
		return;

	UDefaultSaveGame* SaveGame = SaveManager->GetSharedSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithLoadedSharedSaveGame(), SharedSaveGame is not valid."));
		return;
	}

	CompleteSharedLoad(SHARED_SAVE_FILE_NAME, 0, SaveGame, this);
}

void USaver::BeginWithNewSharedSaveGame()
{
	if (!IsActive())
		return;

	UDefaultSaveGame* SaveGame = SaveManager->GetSharedSaveGameInstance();

	if (!IsValid(SaveGame))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("BeginWithNewSharedSaveGame(), SharedSaveGame is not valid."));
		return;
	}

	OnBeginWithNewSharedSaveGameEvent(SaveGame);
	OnBeginWithNewSharedSaveGame.Broadcast(SaveGame);
}

void USaver::SaveAllBehaviours()
{
	for (USaveBehaviour* Behaviour : SaveBehaviours)
	{
		if (IsValid(Behaviour))
		{
			USaveDataBase* SaveData = Behaviour->GetSaveDataInstance();
			Behaviour->PrepareForSerializationNative(SaveData);
			Behaviour->Execute_Save(Behaviour, SaveData);
			PushDataToSaveGame(SaveData);
		}
	}
}

void USaver::SaveAllSharedBehaviours()
{
	for (USaveBehaviour* Behaviour : SharedSaveBehaviours)
	{
		if (IsValid(Behaviour))
		{
			USaveDataBase* SaveData = Behaviour->GetSaveDataInstance();
			Behaviour->PrepareForSerializationNative(SaveData);
			Behaviour->Execute_Save(Behaviour, SaveData);
			PushDataToSaveGame(SaveData, true);
		}
	}
}

void USaver::LoadAllBehaviours()
{
	for (USaveBehaviour* Behaviour : SaveBehaviours)
	{
		if (IsValid(Behaviour))
		{
			USaveDataBase* SaveData = Behaviour->GetSaveDataInstance();
			Behaviour->PrepareForDeserializationNative(SaveData);
			PullDataFromSaveGame(SaveData);
			Behaviour->Execute_Load(Behaviour, SaveData);
		}
	}
}

void USaver::LoadAllSharedBehaviours()
{
	for (USaveBehaviour* Behaviour : SharedSaveBehaviours)
	{
		if (IsValid(Behaviour))
		{
			USaveDataBase* SaveData = Behaviour->GetSaveDataInstance();
			Behaviour->PrepareForDeserializationNative(SaveData);
			PullDataFromSaveGame(SaveData, true);
			Behaviour->Execute_Load(Behaviour, SaveData);
		}
	}
}

void USaver::InitSaveBehaviours()
{
	for (USaveBehaviour* Behaviour : GetAllSaveBehaviours())
	{
		if (IsValid(Behaviour))
		{
			Behaviour->Init(this);
			Behaviour->BeginPlay();
		}
	}
}

void USaver::EndPlaySaveBehaviours(const EEndPlayReason::Type EndPlayReason) const
{
	for (USaveBehaviour* Behaviour : GetAllSaveBehaviours())
	{
		if (IsValid(Behaviour))
			Behaviour->EndPlay(EndPlayReason);
	}
}

bool USaver::Check() const
{
	return SaveManager != nullptr;
}
