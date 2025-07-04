// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/CheckpointIndexData.h"
#include "Data/SaveData/CheckpointsSaveData.h"
#include "CheckpointsManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogCheckpointSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCheckpointReached
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHECKPOINTSYSTEM_API UCheckpointsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCheckpointReached OnCheckpointSet;

private:
	int32 CheckpointIndex = MIN_int32;
	TMap<FCheckpointIndexData, FTransform> CheckpointsMap;

public:
	UCheckpointsManager();

	UFUNCTION(BlueprintPure)
	FCheckpointsSaveData CreateSaveData() const;

	UFUNCTION(BlueprintCallable)
	void LoadSaveData(FCheckpointsSaveData SaveData);

	UFUNCTION(BlueprintCallable)
	bool TrySetCheckpoint(const FName CheckpointKey, const FTransform CheckpointTransform);

	UFUNCTION(BlueprintPure)
	bool TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform);

	UFUNCTION(BlueprintPure)
	bool TryGetLastCheckpoint(FTransform& OutCheckpointTransform) const;

protected:
	virtual void BeginPlay() override;

private:
	bool ContainsCheckpointWithKey(const FName CheckpointKey);

	FTransform GetCheckpointTransformWithKey(const FName CheckpointKey);
};
