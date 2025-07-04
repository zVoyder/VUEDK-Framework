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

	/**
	 * @brief Creates a save data object containing all checkpoints.
	 * @return The checkpoints save data.
	 */
	UFUNCTION(BlueprintPure)
	FCheckpointsSaveData CreateSaveData() const;

	/**
	 * @brief Loads checkpoints from the provided save data.
	 * @param SaveData The save data to load checkpoints from.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSaveData(FCheckpointsSaveData SaveData);

	/**
	 * @brief Tries to set a checkpoint with the given key and transform.
	 * @param CheckpointKey The key identifying the checkpoint.
	 * @param CheckpointTransform The transform to associate with the checkpoint.
	 * @return True if the checkpoint was set successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TrySetCheckpoint(const FName CheckpointKey, const FTransform CheckpointTransform);

	/**
	 * @brief Tries to get the transform of a checkpoint by key.
	 * @param CheckpointKey The key identifying the checkpoint.
	 * @param OutCheckpointTransform The output transform of the checkpoint if found.
	 * @return True if the checkpoint was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform);

	/**
	 * @brief Tries to get the transform of the last checkpoint.
	 * @param OutCheckpointTransform The output transform of the last checkpoint if found.
	 * @return True if the last checkpoint was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool TryGetLastCheckpoint(FTransform& OutCheckpointTransform) const;

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Checks if a checkpoint with the given key exists.
	 * @param CheckpointKey The key identifying the checkpoint.
	 * @return True if the checkpoint exists, false otherwise.
	 */
	bool ContainsCheckpointWithKey(const FName CheckpointKey);

	/**
	 * @brief Gets the transform of a checkpoint with the specified key.
	 * @param CheckpointKey The key identifying the checkpoint.
	 * @return The transform of the checkpoint if it exists, otherwise returns an identity transform.
	 */
	FTransform GetCheckpointTransformWithKey(const FName CheckpointKey);
};
