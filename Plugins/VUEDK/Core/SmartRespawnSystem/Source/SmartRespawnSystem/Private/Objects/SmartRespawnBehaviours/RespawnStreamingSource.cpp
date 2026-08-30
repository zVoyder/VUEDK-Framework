// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/SmartRespawnBehaviours/RespawnStreamingSource.h"

#include "SmartRespawnSystem.h"

ARespawnStreamingSource::ARespawnStreamingSource()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StreamingSource = CreateDefaultSubobject<UWorldPartitionStreamingSourceComponent>(TEXT("StreamingSource"));
}

void ARespawnStreamingSource::Init(APlayerController* InPlayerController, const FGameplayTag InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot)
{
	UE_LOG(LogSmartRespawnSystem, Display, TEXT("ARespawnStreamingSource::Init: Initializing respawn streaming source for player controller %s with respawn tag %s."), *InPlayerController->GetName(), *InRespawnTag.ToString());
	PlayerController = InPlayerController;
	RespawnTag = InRespawnTag;
	RespawnSnapshot = InRespawnSnapshot;
}

bool ARespawnStreamingSource::IsStreamingCompleted() const
{
	return StreamingSource->IsStreamingCompleted();
}

void ARespawnStreamingSource::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsStreamingCompleted())
	{
		UE_LOG(LogSmartRespawnSystem, Display, TEXT("ARespawnStreamingSource::Tick: Streaming completed for player controller %s with respawn tag %s."), *PlayerController->GetName(), *RespawnTag.ToString());
		OnRespawnStreamingSourceCompleted.ExecuteIfBound(PlayerController, RespawnTag, RespawnSnapshot);
		OnRespawnStreamingSourceCompleted.Unbind();
		Destroy();
	}
}
