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
	UFUNCTION(BlueprintPure, Category = "BetterUI|Utility")
	static FString GetProjectVersion();

	UFUNCTION(BlueprintPure, Category = "BetterUI|Utility")
	static FString GetProjectName();
};
