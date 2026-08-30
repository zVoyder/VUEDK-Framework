// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructUtils/PropertyBag.h"
#include "GOAPBlackboardData.generated.h"

UCLASS()
class VGOAP_API UGOAPBlackboardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FInstancedPropertyBag BlackboardProperties;
};
