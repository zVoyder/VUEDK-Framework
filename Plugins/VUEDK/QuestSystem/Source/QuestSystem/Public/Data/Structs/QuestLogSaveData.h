// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveData.h"
#include "QuestLogSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FQuestLogSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName TrackedQuestFName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FQuestSaveData> Quests;

	FQuestLogSaveData() = default;
};
