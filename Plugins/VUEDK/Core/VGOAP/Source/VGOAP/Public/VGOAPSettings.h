// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/GOAPBlackboardData.h"
#include "UObject/Object.h"
#include "VGOAPSettings.generated.h"

UCLASS(Config = VGOAPSettings, DefaultConfig, meta = (DisplayName = "VGOAP Settings"))
class VGOAP_API UVGOAPSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TMap<FGameplayTag, TSoftObjectPtr<UGOAPBlackboardData>> GlobalBlackboards;

public:
	explicit UVGOAPSettings(const FObjectInitializer& ObjectInitializer);
};
