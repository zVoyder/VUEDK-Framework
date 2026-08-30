// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/RPGItemBaseGenerationData.h"
#include "Engine/DataAsset.h"
#include "RPGGearItemGenerationSet.generated.h"

UCLASS()
class RPGSYSTEM_API URPGGearItemGenerationSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<URPGItemBaseGenerationData*> Generations;
};
