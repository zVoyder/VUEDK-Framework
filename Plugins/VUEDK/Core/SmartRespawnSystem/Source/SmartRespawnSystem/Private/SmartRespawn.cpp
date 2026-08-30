// Copyright VUEDK, Inc. All Rights Reserved.

#include "SmartRespawn.h"
#include "SmartRespawnSystem.h"
#include "GameFramework/PlayerState.h"
//#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Utility/SmartRespawnUtility.h"

USmartRespawn::USmartRespawn()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FSmartRespawnSnapshot USmartRespawn::CreateRespawnSnapshot()
{
	FSmartRespawnSnapshot RespawnSnapshot;
	RespawnSnapshot.Priority = Priority;
	RespawnSnapshot.RespawnTransform = GetComponentTransform();
	RespawnSnapshot.Timestamp = FDateTime::Now();
	RespawnSnapshot.Owner = GetOwner();
	RespawnSnapshot.SmartRespawn = this;
	return RespawnSnapshot;
}

bool USmartRespawn::RegisterRespawn(const APlayerState* PlayerState, const bool bOverrideExisting)
{
	const FSmartRespawnSnapshot RespawnSnapshot = CreateRespawnSnapshot();
	return USmartRespawnUtility::RegisterPlayerRespawn(PlayerState, RespawnTag, RespawnSnapshot, bOverrideExisting);
}

bool USmartRespawn::UnregisterRespawn(const APlayerState* PlayerState) const
{
	return USmartRespawnUtility::UnregisterPlayerRespawn(PlayerState, RespawnTag);
}

void USmartRespawn::NotifyStartRespawn(const APlayerController* PlayerController, const FSmartRespawnSnapshot& RespawnSnapshot) const
{
	OnRespawnStarted.Broadcast(PlayerController, RespawnTag, RespawnSnapshot);
}

void USmartRespawn::NotifyFinishRespawn(const APlayerController* PlayerController, const FSmartRespawnSnapshot& RespawnSnapshot, const bool bSuccess) const
{
	OnRespawnFinished.Broadcast(PlayerController, RespawnTag, RespawnSnapshot, bSuccess);
}
