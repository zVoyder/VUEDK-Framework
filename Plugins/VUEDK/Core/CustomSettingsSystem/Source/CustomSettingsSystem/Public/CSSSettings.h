// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/CustomOptionData.h"
#include "Data/PresetOptionsData.h"
#include "CSSSettings.generated.h"

UCLASS(Config = CSSSettings, DefaultConfig, meta = (DisplayName = "Custom Game User Settings"))
class CUSTOMSETTINGSSYSTEM_API UCSSSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Default Settings")
	bool bAutoDetectResolution = true;
	UPROPERTY(Config, EditAnywhere, meta = (EditCondition = "!bAutoDetectResolution"), Category = "Default Settings")
	FIntPoint DefaultResolution = FIntPoint(1920, 1080);
	UPROPERTY(Config, EditAnywhere, Category = "Default Settings")
	float DefaultFrameRateLimit = 360.0f;
	UPROPERTY(Config, EditAnywhere, Category = "Default Settings")
	bool DefaultVSync = true;
	UPROPERTY(Config, EditAnywhere, Category = "Custom Settings")
	TMap<FGameplayTag, FCustomOptionData> CustomSettingsMap;
	UPROPERTY(Config, EditAnywhere, Category = "Presets")
	bool bUsePresets = true;
	UPROPERTY(Config, EditAnywhere, meta = (EditCondition = "bUsePresets"), Category = "Presets")
	FGameplayTag DefaultPreset;
	UPROPERTY(Config, EditAnywhere, meta = (EditCondition = "bUsePresets"), Category = "Presets")
	TMap<FGameplayTag, FPresetOptionsData> PresetSettingsMap;
};
