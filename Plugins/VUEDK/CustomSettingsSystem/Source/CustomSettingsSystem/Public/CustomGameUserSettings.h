// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CSSSettings.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCustomOptionChanged,
	const FGameplayTag&, Tag,
	float, Value
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCustomSettingsApplied
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCustomSettingsResetToDefaults
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCustomSettingsResetToCurrent
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCustomSettingsResetToFile
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCustomSettingsUINeedsUpdate
);

UCLASS(NotBlueprintable, BlueprintType)
class CUSTOMSETTINGSSYSTEM_API UCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomOptionChanged OnCustomOptionChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsApplied OnCustomSettingsApplied;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsResetToDefaults OnResetAllToDefaults;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsResetToCurrent OnResetVideoToDefaults;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsResetToCurrent OnResetCustomOptionsToDefaults;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsResetToCurrent OnCustomSettingsResetToCurrent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsResetToFile OnCustomSettingsResetToFile;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCustomSettingsUINeedsUpdate OnCustomSettingsUINeedsUpdate;

	UPROPERTY(Config)
	TMap<FGameplayTag, float> CurrentSettingsMap;

private:
	UPROPERTY(Config)
	bool bIsInitialized = false;
	UPROPERTY()
	UCSSSettings* CSSSettings;

public:
	/**
	 * @brief Loads the user settings from file or memory.
	 * @param bForceReload If true, forces a reload from file even if already loaded.
	 */
	virtual void LoadSettings(bool bForceReload = false) override;
	
	/**
	 * @brief Sets a custom option value associated with the specified tag.
	 * @param Tag The gameplay tag identifying the custom option.
	 * @param Value The value to set for the custom option.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetCustomOption(const FGameplayTag Tag, float Value);

	/**
	 * @brief Tries to get the value of a custom option by tag.
	 * @param Tag The gameplay tag identifying the custom option.
	 * @param OutValue The output value if the option is found.
	 * @return True if the custom option was found, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Settings)
	bool TryGetCustomOption(const FGameplayTag Tag, float& OutValue) const;

	/**
	 * @brief Sets all user settings and custom options to their default values.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetAllToDefaults();

	/**
	 * @brief Sets all user settings to their default values.
	 */
	virtual void SetToDefaults() override;

	/**
	 * @brief Sets all custom options to their default values.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetAllCustomOptionsToDefaults();

	/**
	 * @brief Sets the specified custom options to their default values.
	 * @param Tags The array of gameplay tags identifying the custom options to reset.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetCustomOptionsToDefaults(TArray<FGameplayTag> Tags);
	
	/**
	 * @brief Resets all settings to the current in-memory values.
	 */
	virtual void ResetToCurrentSettings() override;

	/**
	 * @brief Resets all settings to the values stored in the settings file.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void ResetToFileSettings();

	/**
	 * @brief Gets the maximum allowed value for a custom option by tag.
	 * @param Tag The gameplay tag identifying the custom option.
	 * @param OutValue The output maximum value for the custom option.
	 */
	UFUNCTION(BlueprintPure, Category = Settings)
	void GetCustomOptionMaxValue(const FGameplayTag Tag, float& OutValue);

	/**
	 * @brief Gets the minimum allowed value for a custom option by tag.
	 * @param Tag The gameplay tag identifying the custom option.
	 * @param OutValue The output minimum value for the custom option.
	 */
	UFUNCTION(BlueprintPure, Category = Settings)
	void GetCustomOptionMinValue(const FGameplayTag Tag, float& OutValue);

	/**
	 * @brief Applies all user settings, optionally checking for command line overrides and applying resolution settings.
	 * @param bCheckForCommandLineOverrides If true, checks for command line overrides before applying settings.
	 * @param bApplyResolutionSettings If true, applies resolution-related settings as well.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void ApplyAllSettings(const bool bCheckForCommandLineOverrides, const bool bApplyResolutionSettings);

private:
	/**
	 * @brief Returns the settings object associated with this user settings instance.
	 * @return The settings object (UCSSSettings pointer).
	 */
	const UCSSSettings* GetSettings();

	/**
	 * @brief Clamps a value for a custom option according to its tag and settings.
	 * @param Value The value to clamp.
	 * @param Tag The gameplay tag identifying the custom option.
	 * @param Settings The settings object to use for clamping.
	 * @return The clamped value.
	 */
	static float ClampValue(const float Value, const FGameplayTag Tag, const UCSSSettings* Settings);
};
