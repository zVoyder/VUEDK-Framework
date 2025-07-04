// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CheckpointsManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CSUtility.generated.h"

UCLASS()
class CHECKPOINTSYSTEM_API UCSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UCheckpointsManager* CurrCheckpointsManager;

public:
	/**
	 * @brief Initializes the current checkpoints manager.
	 * @param CheckpointsManager The checkpoints manager to initialize with.
	 */
	static void Init(UCheckpointsManager* CheckpointsManager);

	/**
	 * @brief Tries to set a checkpoint with the given key and transform.
	 * @param CheckpointKey The key identifying the checkpoint.
	 * @param CheckpointTransform The transform to associate with the checkpoint.
	 * @return True if the checkpoint was set successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	static bool TrySetCheckpoint(const FName CheckpointKey, const FTransform& CheckpointTransform);

	/**
	 * @brief Tries to get the transform of a checkpoint by key.
	 * @param CheckpointKey The key identifying the checkpoint.
	 * @param OutCheckpointTransform The output transform of the checkpoint if found.
	 * @return True if the checkpoint was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	static bool TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform);

	/**
	 * @brief Tries to get the transform of the last checkpoint.
	 * @param OutCheckpointTransform The output transform of the last checkpoint if found.
	 * @return True if the last checkpoint was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	static bool TryGetLastCheckpoint(FTransform& OutCheckpointTransform);

private:
	/**
	 * @brief Checks the internal state of the utility class.
	 * @return True if the state is valid, false otherwise.
	 */
	static bool Check();
};
