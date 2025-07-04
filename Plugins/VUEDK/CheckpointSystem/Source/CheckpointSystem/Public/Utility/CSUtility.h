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
	static void Init(UCheckpointsManager* CheckpointsManager);

	UFUNCTION(BlueprintCallable)
	static bool TrySetCheckpoint(const FName CheckpointKey, const FTransform& CheckpointTransform);

	UFUNCTION(BlueprintPure)
	static bool TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform);

	UFUNCTION(BlueprintPure)
	static bool TryGetLastCheckpoint(FTransform& OutCheckpointTransform);

private:
	static bool Check();
};
