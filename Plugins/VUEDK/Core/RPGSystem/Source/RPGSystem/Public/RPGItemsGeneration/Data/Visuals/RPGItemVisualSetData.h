// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGItemVisualDetails.h"
#include "RPGItemVisualSetData.generated.h"

UCLASS()
class RPGSYSTEM_API URPGItemVisualSetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<FRPGItemVisualDetails> Visuals;
};
