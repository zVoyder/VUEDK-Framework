// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/PoolsManagersData.h"
#include "UObject/Object.h"
#include "ObjectPoolSettings.generated.h"

UCLASS(Config = ObjectPoolSettings, DefaultConfig, meta = (DisplayName = "Object Pool Settings"))
class OBJECTPOOL_API UObjectPoolSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray<FPoolsManagersData> PoolsManagers;
	
public:
	explicit UObjectPoolSettings(const FObjectInitializer& ObjectInitializer);
};
