// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Enums/QuestStatus.h"
#include "Data/Enums/QuestType.h"
#include "QuestEntryData.generated.h"

USTRUCT(BlueprintType)
struct FQuestEntryData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType QuestType = EQuestType::Parallel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestStatus InitialQuestStatus = EQuestStatus::Active;

	FQuestEntryData() = default;
};
