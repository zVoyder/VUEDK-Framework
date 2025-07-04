// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/AutoSaveManagerData.h"
#include "Data/Structs/SaveManagerData.h"
#include "UObject/Object.h"
#include "SaveSystemSettings.generated.h"

UCLASS(Config = SaveSystemSettings, DefaultConfig, meta = (DisplayName = "Save System Settings"))
class SAVESYSTEM_API USaveSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FSaveManagerData SaveManagerData;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	bool bEnableAutoSave;
	UPROPERTY(Config, EditAnywhere, Category = Settings, meta = (EditCondition = "bEnableAutoSave", EditConditionHides))
	FAutoSaveManagerData AutoSaveManagerData;

public:
	explicit USaveSystemSettings(const FObjectInitializer& ObjectInitializer);
};
