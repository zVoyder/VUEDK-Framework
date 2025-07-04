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
	UFUNCTION(BlueprintCallable, Category = Settings)
	static UCustomGameUserSettings* GetCustomUserSettings();

private:
	static bool Check();
};
