// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGItemsGeneration/Data/Visuals/RPGItemVisualSetData.h"
#include "RPGItemBaseGenerationData.generated.h"

UCLASS(BlueprintType, Blueprintable)
class RPGSYSTEM_API URPGItemBaseGenerationData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPGItemVisualSetData* ItemVisualDetailsSet;
};
