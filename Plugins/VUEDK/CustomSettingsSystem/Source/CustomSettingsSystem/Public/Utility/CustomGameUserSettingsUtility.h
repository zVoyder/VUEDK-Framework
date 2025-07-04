// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameUserSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomGameUserSettingsUtility.generated.h"

UCLASS()
class CUSTOMSETTINGSSYSTEM_API UCustomUserSettingsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UCustomGameUserSettings* UserSettings;

public:
	/**
	 * Get the custom game user settings instance.
	 * @return The custom game user settings instance, or nullptr if it is not initialized.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	static UCustomGameUserSettings* GetCustomUserSettings();

private:
	/**
	 * Initialize the custom game user settings instance.
	 * @return True if the instance was successfully initialized, false otherwise.
	 */
	static bool Check();
};
