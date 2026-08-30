// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEffectsCue.h"
#include "Engine/DataAsset.h"
#include "GameEffectsCueData.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UGameEffectsCueData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "GameEffects")
	UGameEffectsCue* GameEffectsCue;
};
