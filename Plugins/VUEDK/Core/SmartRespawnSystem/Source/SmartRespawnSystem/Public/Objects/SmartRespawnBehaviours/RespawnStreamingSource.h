// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SmartRespawnBehaviour.h"
#include "Components/WorldPartitionStreamingSourceComponent.h"
#include "GameFramework/Actor.h"
#include "RespawnStreamingSource.generated.h"

DECLARE_DELEGATE_ThreeParams(
	FOnRespawnStreamingSourceCompleted,
	APlayerController*,
	const FGameplayTag&,
	const FSmartRespawnSnapshot&
);

UCLASS()
class SMARTRESPAWNSYSTEM_API ARespawnStreamingSource : public AActor
{
	GENERATED_BODY()

public:
	FOnRespawnStreamingSourceCompleted OnRespawnStreamingSourceCompleted;
	UPROPERTY()
	TObjectPtr<UWorldPartitionStreamingSourceComponent> StreamingSource;

private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY()
	FGameplayTag RespawnTag;
	UPROPERTY()
	FSmartRespawnSnapshot RespawnSnapshot;

public:
	ARespawnStreamingSource();

	void Init(APlayerController* InPlayerController, FGameplayTag InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot);
	
	bool IsStreamingCompleted() const;

	virtual void Tick(float DeltaSeconds) override;
};
