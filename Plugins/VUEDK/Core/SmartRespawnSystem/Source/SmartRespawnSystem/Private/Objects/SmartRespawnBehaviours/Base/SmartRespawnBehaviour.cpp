// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/SmartRespawnBehaviours/Base/SmartRespawnBehaviour.h"
#include "SmartRespawn.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/SmartRespawnUtility.h"

void USmartRespawnBehaviour::StartRespawn(APlayerController* InPlayerController, const FGameplayTag InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot)
{
	PlayerController = InPlayerController;
	RespawnTag = InRespawnTag;
	RespawnSnapshot = InRespawnSnapshot;
	SmartRespawn = RespawnSnapshot.SmartRespawn.LoadSynchronous();
	Owner = RespawnSnapshot.Owner.LoadSynchronous();

	if (IsValid(Owner) && !IsValid(SmartRespawn))
		SmartRespawn = Cast<USmartRespawn>(Owner->GetComponentByClass(USmartRespawn::StaticClass()));
	
	NotifyStartRespawn();
}

USmartRespawn* USmartRespawnBehaviour::GetSmartRespawn() const
{
	return SmartRespawn;
}

AActor* USmartRespawnBehaviour::GetOwner() const
{
	return IsValid(Owner) ? Owner : IsValid(SmartRespawn) ? SmartRespawn->GetOwner() : nullptr;
}

void USmartRespawnBehaviour::OnRespawnStart_Implementation(const APlayerController* InPlayerController, const FGameplayTag& InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot)
{
	FinishExecute(true);
}

void USmartRespawnBehaviour::OnRespawnFinish_Implementation(const APlayerController* InPlayerController, const FGameplayTag& InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot, const bool bSuccess)
{
}

void USmartRespawnBehaviour::ApplyRespawnSnapshot(const FSmartRespawnSnapshot& InRespawnSnapshot, const bool bFinish, const bool bSuccess)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		FinishExecute(false);
		return;
	}

	const AGameModeBase* GameMode = World->GetAuthGameMode();
	if (!IsValid(GameMode))
	{
		if (bFinish)
			FinishExecute(false);
		return;
	}

	World->FlushLevelStreaming(EFlushLevelStreamingType::Full);
	APawn* Pawn = PlayerController->GetPawn();
	const bool TeleportSuccess = Pawn->TeleportTo(InRespawnSnapshot.RespawnTransform.GetLocation(), InRespawnSnapshot.RespawnTransform.GetRotation().Rotator(), false, true);
	PlayerController->SetControlRotation(InRespawnSnapshot.RespawnTransform.GetRotation().Rotator());

	if (bFinish)
		FinishExecute(bSuccess && TeleportSuccess);
}

void USmartRespawnBehaviour::FinishExecute(const bool bSuccess)
{
	NotifyFinishRespawn(bSuccess);
}

void USmartRespawnBehaviour::NotifyStartRespawn()
{
	OnRespawnStart(PlayerController, RespawnTag, RespawnSnapshot);
	
	OnRespawnStarted.Broadcast(PlayerController, RespawnTag, RespawnSnapshot);
	if (IsValid(SmartRespawn))
		SmartRespawn->NotifyStartRespawn(PlayerController, RespawnSnapshot);
	USmartRespawnUtility::NotifyRespawnStart(PlayerController, RespawnTag, RespawnSnapshot, this);
}

void USmartRespawnBehaviour::NotifyFinishRespawn(const bool bSuccess)
{
	OnRespawnFinish(PlayerController, RespawnTag, RespawnSnapshot, bSuccess);
	
	OnRespawnFinished.Broadcast(PlayerController, RespawnTag, RespawnSnapshot, bSuccess);
	if (IsValid(SmartRespawn))
		SmartRespawn->NotifyFinishRespawn(PlayerController, RespawnSnapshot, bSuccess);
	USmartRespawnUtility::NotifyRespawnFinish(PlayerController, RespawnTag, RespawnSnapshot, this, bSuccess);
}
