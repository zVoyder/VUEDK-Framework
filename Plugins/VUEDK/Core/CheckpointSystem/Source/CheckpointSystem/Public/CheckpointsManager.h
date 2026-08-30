// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/CheckpointIndexData.h"
#include "CheckpointsManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCheckpointSystem, Log, All);

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
	 * @param OutCheckpointKey The output checkpoint key of the last checkpoint if found.
	 * @return True if the last checkpoint was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool TryGetLastCheckpoint(FTransform& OutCheckpointTransform, FName& OutCheckpointKey) const;
	
	TMap<FCheckpointIndexData, FTransform> GetCheckpointsMap() const;
	
	int32 GetCheckpointIndex() const;

	void SetCheckpointIndex(const int32 InCheckpointIndex);

	void SetCheckpointsMap(const TMap<FCheckpointIndexData, FTransform>& InCheckpointsMap);

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
