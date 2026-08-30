// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QualityLevel.h"
#include "GameplayTagContainer.h"
#include "QualityType.h"
#include "PresetOptionsData.generated.h"

USTRUCT(BlueprintType)
struct FPresetOptionsData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseShadingQuality"), Category = Setting)
	EQualityLevel ShadingQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseTextureQuality"), Category = Setting)
	EQualityLevel TextureQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseFoliageQuality"), Category = Setting)
	EQualityLevel FoliageQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseVisualEffectsQuality"), Category = Setting)
	EQualityLevel VisualEffectsQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseAntiAliasingQuality"), Category = Setting)
	EQualityLevel AntiAliasingQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseViewDistanceQuality"), Category = Setting)
	EQualityLevel DistanceQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseGlobalIlluminationQuality"), Category = Setting)
	EQualityLevel IlluminationQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseShadowQuality"), Category = Setting)
	EQualityLevel ShadowQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUsePostProcessQuality"), Category = Setting)
	EQualityLevel PostProcessQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseReflectionQuality"), Category = Setting)
	EQualityLevel ReflectionQuality;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseAudioQuality"), Category = Setting)
	EQualityLevel AudioQuality;
	UPROPERTY(EditAnywhere, Category = Setting)
	TMap<FGameplayTag, float> CustomSettingsPresetMap;
	UPROPERTY()
	bool bUseShadingQuality;
	UPROPERTY()
	bool bUseTextureQuality;
	UPROPERTY()
	bool bUseFoliageQuality;
	UPROPERTY()
	bool bUseVisualEffectsQuality;
	UPROPERTY()
	bool bUseAntiAliasingQuality;
	UPROPERTY()
	bool bUseViewDistanceQuality;
	UPROPERTY()
	bool bUseGlobalIlluminationQuality;
	UPROPERTY()
	bool bUseShadowQuality;
	UPROPERTY()
	bool bUsePostProcessQuality;
	UPROPERTY()
	bool bUseReflectionQuality;
	UPROPERTY()
	bool bUseAudioQuality;

	FPresetOptionsData() : ShadingQuality(EQualityLevel::Medium),
	                       TextureQuality(EQualityLevel::Medium),
	                       FoliageQuality(EQualityLevel::Medium),
	                       VisualEffectsQuality(EQualityLevel::Medium),
	                       AntiAliasingQuality(EQualityLevel::Medium),
	                       DistanceQuality(EQualityLevel::Medium),
	                       IlluminationQuality(EQualityLevel::Medium),
	                       ShadowQuality(EQualityLevel::Medium),
	                       PostProcessQuality(EQualityLevel::Medium),
	                       ReflectionQuality(EQualityLevel::Medium),
	                       AudioQuality(EQualityLevel::Medium),
	                       CustomSettingsPresetMap(),
	                       bUseShadingQuality(true),
	                       bUseTextureQuality(true),
	                       bUseFoliageQuality(true),
	                       bUseVisualEffectsQuality(true),
	                       bUseAntiAliasingQuality(true),
	                       bUseViewDistanceQuality(true),
	                       bUseGlobalIlluminationQuality(true),
	                       bUseShadowQuality(true),
	                       bUsePostProcessQuality(true),
	                       bUseReflectionQuality(true),
	                       bUseAudioQuality(false)
	{
	}
};
