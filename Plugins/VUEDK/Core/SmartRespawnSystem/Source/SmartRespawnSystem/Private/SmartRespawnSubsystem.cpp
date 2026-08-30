// Copyright VUEDK, Inc. All Rights Reserved.

#include "SmartRespawnSubsystem.h"
#include "SmartRespawn.h"
#include "SmartRespawnSystem.h"
#include "SmartRespawnSystemSettings.h"
#include "Factories/SmartRespawnFactory.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Utility/SmartRespawnUtility.h"
#if WITH_EDITOR
#include "Engine/PlayerStartPIE.h"
#endif

void USmartRespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	USmartRespawnUtility::Init(this);
	RegisterSaver = USmartRespawnFactory::CreateRegisterSaver(this);
	FGameModeEvents::OnGameModePostLoginEvent().AddUObject(this, &USmartRespawnSubsystem::OnPostLogin);
}

void USmartRespawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
	FGameModeEvents::OnGameModePostLoginEvent().RemoveAll(this);

	if (IsValid(RegisterSaver) && RegisterSaver->OnRegisterRestored.IsBound())
		RegisterSaver->OnRegisterRestored.Unbind();
}

void USmartRespawnSubsystem::SetRegister(const FSmartRespawnRegister& NewRegister)
{
	Register = NewRegister;
}

const FSmartRespawnRegister& USmartRespawnSubsystem::GetRegister() const
{
	return Register;
}

bool USmartRespawnSubsystem::RegisterPlayerRespawn(const APlayerState* Player, const FGameplayTag& RespawnTag, FSmartRespawnSnapshot& RespawnSnapshot, const bool bOverrideExisting)
{
	if (!IsValid(Player))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RegisterRespawn: Invalid player state."));
		return false;
	}

	RespawnSnapshot.Timestamp = FDateTime::Now();
	TMap<FGameplayTag, FSmartRespawnSnapshot>& PlayerRespawns = Register.PlayerRespawns.FindOrAdd(GetPlayerId(Player)).LevelRespawns.FindOrAdd(Player->GetWorld()->GetCurrentLevel()->GetPathName()).Respawns;
	if (PlayerRespawns.Contains(RespawnTag) && !bOverrideExisting)
		return false;

	PlayerRespawns.Add(RespawnTag, RespawnSnapshot);
	OnRespawnRegistered.Broadcast(Player, RespawnTag, RespawnSnapshot);
	NewEntrySave();
	return true;
}

bool USmartRespawnSubsystem::UnregisterPlayerRespawn(const APlayerState* Player, const FGameplayTag& RespawnTag)
{
	if (!IsValid(Player))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::UnregisterRespawn: Invalid player state."));
		return false;
	}

	TMap<FGameplayTag, FSmartRespawnSnapshot>& PlayerRespawns = Register.PlayerRespawns.FindOrAdd(GetPlayerId(Player)).LevelRespawns.FindOrAdd(Player->GetWorld()->GetCurrentLevel()->GetPathName()).Respawns;
	if (const FSmartRespawnSnapshot* FoundSnapshot = PlayerRespawns.Find(RespawnTag))
	{
		OnRespawnUnregistered.Broadcast(Player, RespawnTag, *FoundSnapshot);
		if (PlayerRespawns.Remove(RespawnTag) > 0)
		{
			NewEntrySave();
			return true;
		}
	}

	return false;
}

bool USmartRespawnSubsystem::FindPlayerRespawn(const APlayerState* Player, const TSubclassOf<USmartRespawnSelectorBase> SelectorClass, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	if (!IsValid(Player))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::FindPlayerRespawn: Invalid player state."));
		return false;
	}

	if (!SelectorClass)
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::FindPlayerRespawn: Invalid SelectorClass."));
		return false;
	}

	TMap<FGameplayTag, FSmartRespawnSnapshot> PlayerRespawns;
	if (!GetPlayerRespawns(Player, PlayerRespawns))
		return false;

	USmartRespawnSelectorBase* Selector = USmartRespawnFactory::CreateRespawnSelector(this, SelectorClass);
	if (!IsValid(Selector))
		return false;

	return Selector->ChoosePlayerRespawn(PlayerRespawns, OutRespawnTag, OutRespawnSnapshot);
}

bool USmartRespawnSubsystem::GetPlayerRespawn(const APlayerState* Player, const FGameplayTag& RespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot) const
{
	TMap<FGameplayTag, FSmartRespawnSnapshot> PlayerRespawns;
	if (GetPlayerRespawns(Player, PlayerRespawns))
	{
		if (const FSmartRespawnSnapshot* FoundSnapshot = PlayerRespawns.Find(RespawnTag))
		{
			OutRespawnSnapshot = *FoundSnapshot;
			return true;
		}
	}

	return false;
}

bool USmartRespawnSubsystem::GetPlayerRespawns(const APlayerState* Player, TMap<FGameplayTag, FSmartRespawnSnapshot>& OutRespawns) const
{
	if (!IsValid(Player))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::GetPlayerRespawns: Invalid player state."));
		return false;
	}

	const FName PlayerId = GetPlayerId(Player);
	if (!Register.PlayerRespawns.Contains(PlayerId))
	{
		UE_LOG(LogSmartRespawnSystem, Display, TEXT("USmartRespawnSubsystem::GetPlayerRespawns: No respawns found for player id %s."), *PlayerId.ToString());
		return false;
	}

	const FString LevelPath = Player->GetWorld()->GetCurrentLevel()->GetPathName();
	const TMap<FString, FRespawnLevelValue>& LevelRespawns = Register.PlayerRespawns[PlayerId].LevelRespawns;
	if (!LevelRespawns.Contains(LevelPath))
	{
		UE_LOG(LogSmartRespawnSystem, Display, TEXT("USmartRespawnSubsystem::GetPlayerRespawns: No respawns found for player id %s in level %s."), *PlayerId.ToString(), *Player->GetWorld()->GetCurrentLevel()->GetName());
		return false;
	}

	OutRespawns = LevelRespawns[LevelPath].Respawns;
	return OutRespawns.Num() > 0;
}

bool USmartRespawnSubsystem::ClearPlayerRespawns(const APlayerState* Player)
{
	if (!IsValid(Player))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::ClearPlayerRespawns: Invalid player state."));
		return false;
	}

	if (Register.PlayerRespawns.Remove(GetPlayerId(Player)) > 0)
	{
		NewEntrySave();
		return true;
	}

	return false;
}

void USmartRespawnSubsystem::ClearRegister()
{
	Register.PlayerRespawns.Empty();
	NewEntrySave();
}

void USmartRespawnSubsystem::SaveRegister() const
{
	if (!IsValid(RegisterSaver))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::SaveRegister: RegisterSaver is not valid."));
		return;
	}

	RegisterSaver->SaveRegister(Register);
}

void USmartRespawnSubsystem::RestoreRegister()
{
	if (!IsValid(RegisterSaver))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RestoreRegister: RegisterSaver is not valid."));
		return;
	}

	if (RegisterSaver->IsRestoring())
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RestoreRegister: Trying to restore while already restoring."));
		return;
	}

	RegisterSaver->OnRegisterRestored.BindUObject(this, &USmartRespawnSubsystem::RegisterFinishRestore);
	RegisterSaver->RestoreRegister();
}

bool USmartRespawnSubsystem::RequestRespawnPlayer(APlayerController* PlayerController, TSubclassOf<USmartRespawnSelectorBase> InSelectorClass)
{
	if (!HandleRespawnPlayer(PlayerController, InSelectorClass))
	{
		OnRespawnRequested.Broadcast(false);
		return false;
	}

	OnRespawnRequested.Broadcast(true);
	return true;
}

bool USmartRespawnSubsystem::RequestRespawnPlayerAt(APlayerController* PlayerController, const FGameplayTag& RespawnTag)
{
	if (!HandleRespawnPlayerAt(PlayerController, RespawnTag))
	{
		OnRespawnRequested.Broadcast(false);
		return false;
	}

	OnRespawnRequested.Broadcast(true);
	return true;
}

void USmartRespawnSubsystem::NotifyRespawnStart(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, USmartRespawnBehaviour* RespawnBehaviour) const
{
	OnRespawnStarted.Broadcast(PlayerController, RespawnTag, RespawnSnapshot, RespawnBehaviour);
}

void USmartRespawnSubsystem::NotifyRespawnFinish(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, const bool bSuccess) const
{
	OnRespawnFinished.Broadcast(PlayerController, RespawnTag, RespawnSnapshot, bSuccess);
}

bool USmartRespawnSubsystem::HandleRespawnPlayer(APlayerController* PlayerController, TSubclassOf<USmartRespawnSelectorBase> InSelectorClass)
{
	RespawningPlayer = nullptr;
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RespawnPlayer: Invalid PlayerController."));
		return false;
	}

	if (!IsValid(PlayerController->PlayerState))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RespawnPlayer: PlayerController %s has no valid PlayerState."), *PlayerController->GetName());
		return false;
	}

	TMap<FGameplayTag, FSmartRespawnSnapshot> PlayerRespawns;
	if (!GetPlayerRespawns(PlayerController->PlayerState, PlayerRespawns))
		return false;

	const USmartRespawnSystemSettings* Settings = GetDefault<USmartRespawnSystemSettings>();
	TSubclassOf<USmartRespawnSelectorBase> SelectorClass;
	if (InSelectorClass)
		SelectorClass = InSelectorClass;
	else
		SelectorClass = Settings->RespawnSelectorClass.LoadSynchronous();

	FGameplayTag RelevantRespawnTag;
	FSmartRespawnSnapshot RelevantSnapshot;
	USmartRespawnSelectorBase* Selector = USmartRespawnFactory::CreateRespawnSelector(this, SelectorClass);
	if (!IsValid(Selector))
		return false;

	const FName PlayerId = GetPlayerId(PlayerController->PlayerState);
	if (!Selector->ChoosePlayerRespawn(PlayerRespawns, RelevantRespawnTag, RelevantSnapshot))
	{
		UE_LOG(LogSmartRespawnSystem, Display, TEXT("USmartRespawnSubsystem::RespawnPlayer: No valid respawn found for player id %s."), *PlayerId.ToString());
		return false;
	}

	return StartStreamingSourceRespawn(PlayerController, RelevantRespawnTag, RelevantSnapshot, PlayerId);
}

bool USmartRespawnSubsystem::HandleRespawnPlayerAt(APlayerController* PlayerController, const FGameplayTag& RespawnTag)
{
	RespawningPlayer = nullptr;
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RespawnPlayerAt: Invalid PlayerController."));
		return false;
	}

	if (!IsValid(PlayerController->PlayerState))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::RespawnPlayerAt: PlayerController %s has no valid PlayerState."), *PlayerController->GetName());
		return false;
	}

	const FName PlayerId = GetPlayerId(PlayerController->PlayerState);
	FSmartRespawnSnapshot RespawnSnapshot;
	if (!GetPlayerRespawn(PlayerController->PlayerState, RespawnTag, RespawnSnapshot))
		return false;

	return StartStreamingSourceRespawn(PlayerController, RespawnTag, RespawnSnapshot, PlayerId);
}

bool USmartRespawnSubsystem::StartStreamingSourceRespawn(APlayerController* PlayerController, const FGameplayTag RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, const FName PlayerId)
{
	ARespawnStreamingSource* StreamingSource = USmartRespawnFactory::SpawnRespawnStreamingSource(this, PlayerController, RespawnTag, RespawnSnapshot);
	if (!IsValid(StreamingSource))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::StartStreamingSourceRespawn: Failed to spawn respawn streaming source for player id %s."), *PlayerId.ToString());
		return false;
	}

	DisableInput(PlayerController);
	StreamingSource->OnRespawnStreamingSourceCompleted.BindUObject(this, &USmartRespawnSubsystem::OnRespawnStreamingSourceCompleted);
	return true;
}

void USmartRespawnSubsystem::OnPostLogin(AGameModeBase* GameModeBase, APlayerController* PlayerController)
{
	PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &USmartRespawnSubsystem::OnPossessedPawnChanged);
}

void USmartRespawnSubsystem::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	NewPawn->GetController()->OnPossessedPawnChanged.RemoveDynamic(this, &USmartRespawnSubsystem::OnPossessedPawnChanged);
	InitStartRespawnPlayer(Cast<APlayerController>(NewPawn->GetController()));
}

void USmartRespawnSubsystem::InitStartRespawnPlayer(APlayerController* PlayerController)
{
	if (!IsValid(RegisterSaver))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnSubsystem::InitStartRespawnPlayer: RegisterSaver is not valid."));
		return;
	}

	RespawningPlayer = PlayerController;
	if (RegisterSaver->IsRestored())
	{
		OnPostLoginRestoreFinished.Broadcast(true);
		PostLoginRespawnPlayer();
		return;
	}

	PlayerController->GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USmartRespawnSubsystem::PostLoginRestore);
}

void USmartRespawnSubsystem::NewEntrySave() const
{
	if (!GetDefault<USmartRespawnSystemSettings>()->bNewEntrySave)
		return;

	SaveRegister();
}

void USmartRespawnSubsystem::RegisterFinishRestore(const bool bSuccess) const
{
	OnRestoreFinished.Broadcast(bSuccess);
	RegisterSaver->OnRegisterRestored.Unbind();
	UE_LOG(LogSmartRespawnSystem, Display, TEXT("USmartRespawnSubsystem::RegisterFinishRestore: Register restore finished with success = %s."), bSuccess ? TEXT("true") : TEXT("false"));
}

void USmartRespawnSubsystem::PostLoginRegisterFinishRestore(const bool bSuccess)
{
	OnPostLoginRestoreFinished.Broadcast(bSuccess);
	RegisterFinishRestore(bSuccess);
	PostLoginRespawnPlayer();
}

void USmartRespawnSubsystem::PostLoginRespawnPlayer()
{
	if (!PostLoginShouldRespawnPlayer())
	{
		OnPostLoginRespawnRequested.Broadcast(false);
		return;
	}

	const bool bSuccess = RequestRespawnPlayer(RespawningPlayer);
	OnPostLoginRespawnRequested.Broadcast(bSuccess);
}

void USmartRespawnSubsystem::PostLoginRestore()
{
	if (!GetDefault<USmartRespawnSystemSettings>()->bPostLoginRestore)
		return;

	RegisterSaver->OnRegisterRestored.BindUObject(this, &USmartRespawnSubsystem::PostLoginRegisterFinishRestore);
	RegisterSaver->RestoreRegister();
}

bool USmartRespawnSubsystem::PostLoginShouldRespawnPlayer() const
{
	const uint32 TargetContexts = GetDefault<USmartRespawnSystemSettings>()->PostLoginRespawnContexts;
#if WITH_EDITOR
	if (IsPlayFromHere(RespawningPlayer))
	{
		UE_LOG(LogSmartRespawnSystem, Display, TEXT("USmartRespawnSubsystem::PostLoginShouldRespawnPlayer: Player is using Play From Here, skipping post-login respawn."));
		return false;
	}
	return (TargetContexts & 1 << static_cast<uint8>(EPostLoginRespawnContext::Editor)) != 0;
#else
	switch (const EBuildConfiguration BuildConfig = FApp::GetBuildConfiguration())
	{
	case EBuildConfiguration::Debug:
	case EBuildConfiguration::DebugGame:
		return (TargetContexts & 1 << static_cast<uint8>(EPostLoginRespawnContext::DebugGame)) != 0;
		break;
	case EBuildConfiguration::Development:
		return (TargetContexts & 1 << static_cast<uint8>(EPostLoginRespawnContext::Development)) != 0;
		break;
	case EBuildConfiguration::Shipping:
	case EBuildConfiguration::Test:
		return (TargetContexts & 1 << static_cast<uint8>(EPostLoginRespawnContext::Shipping)) != 0;
		break;
	case EBuildConfiguration::Unknown:
	default: ;
	}

	return false;
#endif
}

void USmartRespawnSubsystem::EnableInput(APlayerController* PlayerController)
{
	const USmartRespawnSystemSettings* Settings = GetDefault<USmartRespawnSystemSettings>();
	if (Settings->bRespawnDisableInput)
	{
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->EnableInput(PlayerController);
	}
}

void USmartRespawnSubsystem::DisableInput(APlayerController* PlayerController)
{
	const USmartRespawnSystemSettings* Settings = GetDefault<USmartRespawnSystemSettings>();
	if (Settings->bRespawnDisableInput)
	{
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->DisableInput(PlayerController);
		PlayerController->FlushPressedKeys();
		PlayerController->StopMovement();
	}
}

FName USmartRespawnSubsystem::GetPlayerId(const APlayerState* Player)
{
	const TSharedPtr<const FUniqueNetId> NetId = Player->GetUniqueId().GetUniqueNetId();

	// If the player has a valid unique net ID, use it as the player ID. Otherwise, fall back to using the player's name
	if (NetId.IsValid() && NetId->GetType() != TEXT("NULL"))
		return FName(*NetId->ToString());

	return FName("LocalPlayerId");
}

bool USmartRespawnSubsystem::IsPlayFromHere(APlayerController* PlayerController)
{
#if WITH_EDITOR
	if (!IsValid(PlayerController))
		return false;

	const UWorld* World = PlayerController->GetWorld();
	if (!IsValid(World))
		return false;

	AGameModeBase* GameMode = World->GetAuthGameMode();
	if (!IsValid(GameMode))
		return false;

	const AActor* SpawnPlayerStart = GameMode->FindPlayerStart(PlayerController);
	if (!IsValid(SpawnPlayerStart))
		return false;

	return SpawnPlayerStart->IsA<APlayerStartPIE>();
#else
	return false;
#endif
}

void USmartRespawnSubsystem::OnRespawnStreamingSourceCompleted(APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot)
{
	USmartRespawnBehaviour* RespawnBehaviour = USmartRespawnFactory::CreateRespawnBehaviour(this, RespawnSnapshot);
	if (!IsValid(RespawnBehaviour))
		return;

	RespawnBehaviour->StartRespawn(PlayerController, RespawnTag, RespawnSnapshot);
	EnableInput(PlayerController);
}
