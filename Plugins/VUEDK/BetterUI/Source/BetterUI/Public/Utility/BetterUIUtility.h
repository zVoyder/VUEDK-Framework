// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterUIUtility.generated.h"

UCLASS()
class BETTERUI_API UBetterUIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns the current project version as a string.
	 * @return The project version.
	 */
	UFUNCTION(BlueprintPure, Category = "BetterUI|Utility")
	static FString GetProjectVersion();

	/**
	 * @brief Returns the current project name as a string.
	 * @return The project name.
	 */
	UFUNCTION(BlueprintPure, Category = "BetterUI|Utility")
	static FString GetProjectName();
};
