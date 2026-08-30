// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameUserSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomSettingsUtility.generated.h"

UCLASS()
class CUSTOMSETTINGSSYSTEM_API UCustomSettingsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Gets the custom game user settings instance.
	 * @return The custom game user settings instance, or nullptr if it is not initialized.
	 */
	UFUNCTION(BlueprintCallable, Category = Settings)
	static UCustomGameUserSettings* GetCustomUserSettings();
};
