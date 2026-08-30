// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/SmartRespawnUtility.h"

TWeakObjectPtr<USmartRespawnSubsystem> USmartRespawnUtility::SmartRespawnSubsystem = nullptr;

void USmartRespawnUtility::Init(USmartRespawnSubsystem* InSmartRespawnSubsystem)
{
	SmartRespawnSubsystem = InSmartRespawnSubsystem;
}

void USmartRespawnUtility::SaveRegister()
{
	if (!Check())
		return;

	SmartRespawnSubsystem.Get()->SaveRegister();
}

void USmartRespawnUtility::LoadRegister()
{
	if (!Check())
		return;

	SmartRespawnSubsystem.Get()->RestoreRegister();
}

void USmartRespawnUtility::SetRespawnsRegister(const FSmartRespawnRegister& NewRegister)
{
	if (!Check())
		return;

	SmartRespawnSubsystem.Get()->SetRegister(NewRegister);
}

const FSmartRespawnRegister& USmartRespawnUtility::GetRespawnsRegister()
{
	FSmartRespawnRegister EmptyRegister;
	if (!Check())
		return EmptyRegister;

	return SmartRespawnSubsystem.Get()->GetRegister();
}

bool USmartRespawnUtility::RegisterPlayerRespawn(const APlayerState* Player, const FGameplayTag RespawnTag, FSmartRespawnSnapshot RespawnSnapshot, const bool bOverrideExisting)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->RegisterPlayerRespawn(Player, RespawnTag, RespawnSnapshot, bOverrideExisting);
}

bool USmartRespawnUtility::UnregisterPlayerRespawn(const APlayerState* Player, const FGameplayTag RespawnTag)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->UnregisterPlayerRespawn(Player, RespawnTag);
}

bool USmartRespawnUtility::FindPlayerRespawn(const APlayerState* Player, const TSubclassOf<USmartRespawnSelectorBase> SelectorClass, FGameplayTag& OutFoundRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->FindPlayerRespawn(Player, SelectorClass, OutFoundRespawnTag, OutRespawnSnapshot);
}

bool USmartRespawnUtility::GetPlayerRespawn(const APlayerState* Player, const FGameplayTag RespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->GetPlayerRespawn(Player, RespawnTag, OutRespawnSnapshot);
}

bool USmartRespawnUtility::GetPlayerRespawns(const APlayerState* Player, TMap<FGameplayTag, FSmartRespawnSnapshot>& OutRespawns)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->GetPlayerRespawns(Player, OutRespawns);
}

bool USmartRespawnUtility::ClearPlayerRespawns(const APlayerState* Player)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->ClearPlayerRespawns(Player);
}

void USmartRespawnUtility::ClearRegister()
{
	if (!Check())
		return;

	SmartRespawnSubsystem.Get()->ClearRegister();
}

bool USmartRespawnUtility::RespawnPlayer(APlayerController* PlayerController, const TSubclassOf<USmartRespawnSelectorBase> SelectorClass)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->RequestRespawnPlayer(PlayerController, SelectorClass);
}

bool USmartRespawnUtility::RespawnPlayerAt(APlayerController* PlayerController, const FGameplayTag RespawnTag)
{
	if (!Check())
		return false;

	return SmartRespawnSubsystem.Get()->RequestRespawnPlayerAt(PlayerController, RespawnTag);
}

void USmartRespawnUtility::NotifyRespawnStart(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, USmartRespawnBehaviour* RespawnBehaviour)
{
	if (!Check())
		return;

	SmartRespawnSubsystem.Get()->NotifyRespawnStart(PlayerController, RespawnTag, RespawnSnapshot, RespawnBehaviour);
}

void USmartRespawnUtility::NotifyRespawnFinish(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, USmartRespawnBehaviour* RespawnBehaviour, const bool bSuccess)
{
	if (!Check())
		return;

	SmartRespawnSubsystem.Get()->NotifyRespawnFinish(PlayerController, RespawnTag, RespawnSnapshot, bSuccess);
}

bool USmartRespawnUtility::Check()
{
	return SmartRespawnSubsystem.IsValid();
}
