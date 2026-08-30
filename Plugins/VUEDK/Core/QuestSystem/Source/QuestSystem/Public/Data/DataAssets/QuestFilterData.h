// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestFilterData.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEM_API UQuestFilterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString QuestFilterName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateBrush QuestFilterIcon;
};
