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
	virtual void LoadSettings(bool bForceReload = false) override;
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetCustomOption(const FGameplayTag Tag, float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Settings)
	bool TryGetCustomOption(const FGameplayTag Tag, float& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetAllToDefaults();

	virtual void SetToDefaults() override;

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetAllCustomOptionsToDefaults();

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetCustomOptionsToDefaults(TArray<FGameplayTag> Tags);
	
	virtual void ResetToCurrentSettings() override;

	UFUNCTION(BlueprintCallable, Category = Settings)
	void ResetToFileSettings();

	UFUNCTION(BlueprintPure, Category = Settings)
	void GetCustomOptionMaxValue(const FGameplayTag Tag, float& OutValue);

	UFUNCTION(BlueprintPure, Category = Settings)
	void GetCustomOptionMinValue(const FGameplayTag Tag, float& OutValue);

	UFUNCTION(BlueprintCallable, Category = Settings)
	void ApplyAllSettings(const bool bCheckForCommandLineOverrides, const bool bApplyResolutionSettings);

private:
	const UCSSSettings* GetSettings();

	static float ClampValue(const float Value, const FGameplayTag Tag, const UCSSSettings* Settings);
};
