// Copyright VUEDK, Inc. All Rights Reserved.

#include "CustomGameUserSettings.h"
#include "CSSSettings.h"
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
	OnCustomOptionChanged.Broadcast(Tag, Value);
}

bool UCustomGameUserSettings::TryGetCustomOption(const FGameplayTag Tag, float& OutValue) const
{
	if (!CurrentSettingsMap.Contains(Tag))
		return false;

	OutValue = CurrentSettingsMap[Tag];
	return true;
}

void UCustomGameUserSettings::SetAllToDefaults()
{
	SetToDefaults();
	SetAllCustomOptionsToDefaults();
	OnResetAllToDefaults.Broadcast();
	OnCustomSettingsUINeedsUpdate.Broadcast();
}

void UCustomGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	const UCSSSettings* Settings = GetSettings();
	if (!IsValid(Settings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomGameUserSettings::SetToDefaults: No settings found."));
		return;
	}

	bUseVSync = Settings->DefaultVSync;
	FrameRateLimit = Settings->DefaultFrameRateLimit;
	SetScreenResolution(Settings->DefaultResolution);
	OnCustomSettingsUINeedsUpdate.Broadcast();
	OnResetVideoToDefaults.Broadcast();
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
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomGameUserSettings::SetCustomOptionsToDefaults: No settings found."));
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
