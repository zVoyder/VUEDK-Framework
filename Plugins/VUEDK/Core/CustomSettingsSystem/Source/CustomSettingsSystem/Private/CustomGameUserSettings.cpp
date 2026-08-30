// Copyright VUEDK, Inc. All Rights Reserved.

#include "CustomGameUserSettings.h"
#include "CSSSettings.h"
#include "HAL/IConsoleManager.h"
#include "CustomSettingsSystem.h"

void UCustomGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);
	if (!bIsInitialized)
	{
		bIsInitialized = true;
		SetAllToDefaults();
		ApplyAllSettings(false, true);
	}

	SetAAMethod(AAMethod);
	CachePresetsKeys();
}

void UCustomGameUserSettings::SetCustomOption(const FGameplayTag Tag, float Value)
{
	const UCSSSettings* Settings = GetSettings();

	if (!IsValid(Settings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomGameUserSettings::SetCustomOption: No settings found."));
		return;
	}

	if (!Settings->CustomSettingsMap.Contains(Tag))
		return;

	Value = ClampValue(Value, Tag, Settings);
	CurrentSettingsMap.Add(Tag, Value);

	if (IsUsingPreset())
	{
		FPresetOptionsData Preset;
		int32 PresetIndex = -1;
		if (TryGetPreset(CurrentPreset, Preset, PresetIndex))
		{
			if (Preset.CustomSettingsPresetMap.Contains(Tag)) // If the custom option is part of the preset, disable the preset
				DisablePreset();
		}
	}

	OnCustomOptionChanged.Broadcast(Tag, Value);
}

bool UCustomGameUserSettings::TryGetCustomOption(const FGameplayTag Tag, float& OutValue) const
{
	if (!CurrentSettingsMap.Contains(Tag))
		return false;

	OutValue = CurrentSettingsMap[Tag];
	return true;
}

void UCustomGameUserSettings::DisablePreset()
{
	if (!IsUsingPreset())
		return;

	CurrentPreset = FGameplayTag::EmptyTag;
	OnDisablePreset.Broadcast();
}

void UCustomGameUserSettings::SetSettingsWithPreset(const FGameplayTag PresetTag)
{
	FPresetOptionsData Preset;
	int32 PresetIndex = -1;

	if (!TryGetPreset(PresetTag, Preset, PresetIndex))
		return;

	// Apply quality settings from the preset
	if (Preset.bUseAntiAliasingQuality)
	{
		const int32 AAQuality = static_cast<int32>(Preset.AntiAliasingQuality);
		SetAntiAliasingQuality(AAQuality);
	}

	if (Preset.bUseAudioQuality)
	{
		const int32 AudioQuality = static_cast<int32>(Preset.AudioQuality);
		SetAudioQualityLevel(AudioQuality);
	}

	if (Preset.bUseViewDistanceQuality)
	{
		const int32 DistanceQuality = static_cast<int32>(Preset.DistanceQuality);
		SetViewDistanceQuality(DistanceQuality);
	}

	if (Preset.bUseFoliageQuality)
	{
		const int32 FoliageQuality = static_cast<int32>(Preset.FoliageQuality);
		SetFoliageQuality(FoliageQuality);
	}

	if (Preset.bUseGlobalIlluminationQuality)
	{
		const int32 IlluminationQuality = static_cast<int32>(Preset.IlluminationQuality);
		SetGlobalIlluminationQuality(IlluminationQuality);
	}

	if (Preset.bUsePostProcessQuality)
	{
		const int32 PostProcessQuality = static_cast<int32>(Preset.PostProcessQuality);
		SetPostProcessingQuality(PostProcessQuality);
	}

	if (Preset.bUseReflectionQuality)
	{
		const int32 ReflectionQuality = static_cast<int32>(Preset.ReflectionQuality);
		SetReflectionQuality(ReflectionQuality);
	}

	if (Preset.bUseShadowQuality)
	{
		const int32 ShadowQuality = static_cast<int32>(Preset.ShadowQuality);
		SetShadowQuality(ShadowQuality);
	}

	if (Preset.bUseShadingQuality)
	{
		const int32 ShadingQuality = static_cast<int32>(Preset.ShadingQuality);
		SetShadingQuality(ShadingQuality);
	}

	if (Preset.bUseTextureQuality)
	{
		const int32 TextureQuality = static_cast<int32>(Preset.TextureQuality);
		SetTextureQuality(TextureQuality);
	}

	if (Preset.bUseVisualEffectsQuality)
	{
		const int32 VisualEffectsQuality = static_cast<int32>(Preset.VisualEffectsQuality);
		SetVisualEffectQuality(VisualEffectsQuality);
	}

	// Apply custom settings from the preset
	if (Preset.CustomSettingsPresetMap.Num() > 0)
	{
		for (const auto& Option : Preset.CustomSettingsPresetMap)
			SetCustomOption(Option.Key, Option.Value);
	}

	CurrentPreset = PresetTag;
	OnPresetChanged.Broadcast(PresetTag, PresetIndex);
	OnCustomSettingsUINeedsUpdate.Broadcast();
}

void UCustomGameUserSettings::SetSettingsWithPresetByIndex(const int32 PresetIndex)
{
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings) || PresetIndex < 0)
		return;

	if (PresetIndex >= PresetsKeys.Num())
		return;

	SetSettingsWithPreset(PresetsKeys[PresetIndex]);
}

void UCustomGameUserSettings::SetAllToDefaults()
{
	SetToDefaults();
	SetAllCustomOptionsToDefaults();
	OnResetAllToDefaults.Broadcast();
}

void UCustomGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	SetAAMethodToDefault();
	
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomGameUserSettings::SetToDefaults: No settings found."));
		return;
	}

	bUseVSync = Settings->DefaultVSync;
	FrameRateLimit = Settings->DefaultFrameRateLimit;

	FIntPoint Resolution = Settings->DefaultResolution;
	if (Settings->bAutoDetectResolution)
	{
		FDisplayMetrics DisplayMetrics;
		FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
		Resolution.X = DisplayMetrics.PrimaryDisplayWidth;
		Resolution.Y = DisplayMetrics.PrimaryDisplayHeight;
	}

	SetScreenResolution(Resolution);
	OnResetVideoToDefaults.Broadcast();

	if (Settings->bUsePresets)
	{
		SetSettingsWithPreset(Settings->DefaultPreset);
		return; // Avoid calling the events twice
	}

	DisablePreset();
	OnCustomSettingsUINeedsUpdate.Broadcast();
}

void UCustomGameUserSettings::SetAllCustomOptionsToDefaults()
{
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomGameUserSettings::SetAllCustomOptionsToDefaults: No settings found."));
		return;
	}

	for (const auto Option : Settings->CustomSettingsMap)
	{
		float Value = ClampValue(Option.Value.DefaultValue, Option.Key, Settings);
		CurrentSettingsMap.Add(Option.Key, Value);
	}

	OnCustomSettingsUINeedsUpdate.Broadcast();
	OnResetCustomOptionsToDefaults.Broadcast();
}

void UCustomGameUserSettings::SetCustomOptionsToDefaults(TArray<FGameplayTag> Tags)
{
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings))
	{
		UE_LOG(LogCustomSettingsSystem, Error,
		       TEXT("UCustomGameUserSettings::SetCustomOptionsToDefaults: No settings found."));
		return;
	}

	for (const auto Tag : Tags)
	{
		if (!CurrentSettingsMap.Contains(Tag))
			continue;

		const float Value = ClampValue(Settings->CustomSettingsMap[Tag].DefaultValue, Tag, Settings);
		CurrentSettingsMap[Tag] = Value;
	}

	OnCustomSettingsUINeedsUpdate.Broadcast();
	OnResetCustomOptionsToDefaults.Broadcast();
}

void UCustomGameUserSettings::ResetToCurrentSettings()
{
	Super::ResetToCurrentSettings();
	OnCustomSettingsResetToCurrent.Broadcast();
	OnCustomSettingsUINeedsUpdate.Broadcast();
}

void UCustomGameUserSettings::ResetToFileSettings()
{
	LoadSettings(true);
	OnCustomSettingsResetToFile.Broadcast();
	OnCustomSettingsUINeedsUpdate.Broadcast();
}

bool UCustomGameUserSettings::IsUsingPreset() const
{
	return CurrentPreset != FGameplayTag::EmptyTag;
}

bool UCustomGameUserSettings::TryGetCurrentPresetTag(FGameplayTag& OutPresetTag, int32& OutPresetIndex)
{
	if (!IsUsingPreset())
		return false;

	OutPresetTag = CurrentPreset;
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings) || !Settings->PresetSettingsMap.Contains(CurrentPreset))
		return false;

	OutPresetIndex = PresetsKeys.Find(CurrentPreset);
	return true;
}

bool UCustomGameUserSettings::TryGetPreset(const FGameplayTag PresetTag, FPresetOptionsData& OutPreset, int32& OutPresetIndex)
{
	const UCSSSettings* Settings = GetSettings();

	if (!IsValid(Settings) || !Settings->PresetSettingsMap.Contains(PresetTag))
		return false;

	OutPreset = Settings->PresetSettingsMap[PresetTag];
	OutPresetIndex = PresetsKeys.Find(PresetTag);
	return true;
}

void UCustomGameUserSettings::GetCustomOptionMaxValue(const FGameplayTag Tag, float& OutValue)
{
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings) || !Settings->CustomSettingsMap.Contains(Tag))
		return;

	OutValue = Settings->CustomSettingsMap[Tag].MaxValue;
}

void UCustomGameUserSettings::GetCustomOptionMinValue(const FGameplayTag Tag, float& OutValue)
{
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings) || !Settings->CustomSettingsMap.Contains(Tag))
		return;

	OutValue = Settings->CustomSettingsMap[Tag].MinValue;
}

void UCustomGameUserSettings::ApplyAllSettings(const bool bCheckForCommandLineOverrides, const bool bApplyResolutionSettings)
{
	ApplySettings(bCheckForCommandLineOverrides);

	if (bApplyResolutionSettings)
		ApplyResolutionSettings(bCheckForCommandLineOverrides);

	OnCustomSettingsApplied.Broadcast();
	OnCustomSettingsUINeedsUpdate.Broadcast();
}

void UCustomGameUserSettings::SetQualityLevel(EQualityType QualityType, EQualityLevel QualityLevel, bool bNotify)
{
	const int32 QualityLevelInt = static_cast<int32>(QualityLevel);
	switch (QualityType)
	{
	case EQualityType::ShadingQuality: SetShadingQuality(QualityLevelInt);
		break;
	case EQualityType::TextureQuality: SetTextureQuality(QualityLevelInt);
		break;
	case EQualityType::FoliageQuality: SetFoliageQuality(QualityLevelInt);
		break;
	case EQualityType::VisualEffectsQuality: SetVisualEffectQuality(QualityLevelInt);
		break;
	case EQualityType::AntiAliasingQuality: SetAntiAliasingQuality(QualityLevelInt);
		break;
	case EQualityType::ViewDistanceQuality: SetViewDistanceQuality(QualityLevelInt);
		break;
	case EQualityType::GlobalIlluminationQuality: SetGlobalIlluminationQuality(QualityLevelInt);
		break;
	case EQualityType::ShadowQuality: SetShadowQuality(QualityLevelInt);
		break;
	case EQualityType::AudioQuality: SetAudioQualityLevel(QualityLevelInt);
		break;
	case EQualityType::PostProcessQuality: SetPostProcessingQuality(QualityLevelInt);
		break;
	case EQualityType::ReflectionQuality: SetReflectionQuality(QualityLevelInt);
		break;
	default: ;
	}

	DisablePresetIfQualityOptionChanged(QualityType);

	if (bNotify)
		OnQualityLevelChanged.Broadcast(QualityType, QualityLevel);
}

EQualityLevel UCustomGameUserSettings::GetQualityLevel(const EQualityType QualityType) const
{
	switch (QualityType)
	{
	case EQualityType::ShadingQuality: return static_cast<EQualityLevel>(GetShadingQuality());
	case EQualityType::TextureQuality: return static_cast<EQualityLevel>(GetTextureQuality());
	case EQualityType::FoliageQuality: return static_cast<EQualityLevel>(GetFoliageQuality());
	case EQualityType::VisualEffectsQuality: return static_cast<EQualityLevel>(GetVisualEffectQuality());
	case EQualityType::AntiAliasingQuality: return static_cast<EQualityLevel>(GetAntiAliasingQuality());
	case EQualityType::ViewDistanceQuality: return static_cast<EQualityLevel>(GetViewDistanceQuality());
	case EQualityType::GlobalIlluminationQuality: return static_cast<EQualityLevel>(GetGlobalIlluminationQuality());
	case EQualityType::ShadowQuality: return static_cast<EQualityLevel>(GetShadowQuality());
	case EQualityType::AudioQuality: return static_cast<EQualityLevel>(GetAudioQualityLevel());
	case EQualityType::PostProcessQuality: return static_cast<EQualityLevel>(GetPostProcessingQuality());
	case EQualityType::ReflectionQuality: return static_cast<EQualityLevel>(GetReflectionQuality());
	default: return EQualityLevel::Low;
	}
}

void UCustomGameUserSettings::SetAAMethod(const int32 Method)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod")))
	{
		CVar->Set(Method);
		AAMethod = Method;
	}
}

void UCustomGameUserSettings::SetAAMethodToDefault()
{
	SetAAMethod(GetDefaultAAMethod());
}

int32 UCustomGameUserSettings::GetAAMethod() const
{
	if (const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod")))
		return CVar->GetInt();

	return -1;
}

int32 UCustomGameUserSettings::GetDefaultAAMethod() const
{
	int32 Value = 0;

	GConfig->GetInt(
		TEXT("/Script/Engine.RendererSettings"),
		TEXT("r.AntiAliasingMethod"),
		Value,
		GEngineIni
	);

	return Value;
}

const UCSSSettings* UCustomGameUserSettings::GetSettings()
{
	if (!IsValid(CSSSettings))
	{
		CSSSettings = GetMutableDefault<UCSSSettings>();

		if (!IsValid(CSSSettings))
		{
			UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomGameUserSettings::GetSettings: No settings found."));
			return nullptr;
		}
	}

	return CSSSettings;
}

float UCustomGameUserSettings::ClampValue(const float Value, const FGameplayTag Tag, const UCSSSettings* Settings)
{
	return FMath::Clamp(Value, Settings->CustomSettingsMap[Tag].MinValue, Settings->CustomSettingsMap[Tag].MaxValue);
}

void UCustomGameUserSettings::DisablePresetIfQualityOptionChanged(const EQualityType QualityType)
{
	if (!IsUsingPreset())
		return;

	FPresetOptionsData Preset;
	int32 PresetIndex = -1;
	if (TryGetPreset(CurrentPreset, Preset, PresetIndex))
	{
		switch (QualityType)
		{
		case EQualityType::ShadingQuality:
			if (Preset.bUseShadingQuality)
				DisablePreset();
			break;
		case EQualityType::TextureQuality:
			if (Preset.bUseTextureQuality)
				DisablePreset();
			break;
		case EQualityType::FoliageQuality:
			if (Preset.bUseFoliageQuality)
				DisablePreset();
			break;
		case EQualityType::VisualEffectsQuality:
			if (Preset.bUseVisualEffectsQuality)
				DisablePreset();
			break;
		case EQualityType::AntiAliasingQuality:
			if (Preset.bUseAntiAliasingQuality)
				DisablePreset();
			break;
		case EQualityType::ViewDistanceQuality:
			if (Preset.bUseViewDistanceQuality)
				DisablePreset();
			break;
		case EQualityType::GlobalIlluminationQuality:
			if (Preset.bUseGlobalIlluminationQuality)
				DisablePreset();
			break;
		case EQualityType::ShadowQuality:
			if (Preset.bUseShadowQuality)
				DisablePreset();
			break;
		case EQualityType::AudioQuality:
			if (Preset.bUseAudioQuality)
				DisablePreset();
			break;
		case EQualityType::PostProcessQuality:
			if (Preset.bUsePostProcessQuality)
				DisablePreset();
			break;
		case EQualityType::ReflectionQuality:
			if (Preset.bUseReflectionQuality)
				DisablePreset();
			break;
		default: ;
		}
	}
}

void UCustomGameUserSettings::CachePresetsKeys()
{
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings))
		return;

	Settings->PresetSettingsMap.GetKeys(PresetsKeys);
}
