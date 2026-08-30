// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PerceptionConfigData.generated.h"

class USenseBase;

UCLASS(Blueprintable, BlueprintType)
class SIMPLEPERCEPTIONSYSTEM_API UPerceptionConfigData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Perception")
	TArray<USenseBase*> Senses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.f, UIMin = 0.f), Category = "Perception")
	float MemoryDuration = 10.f;
};
