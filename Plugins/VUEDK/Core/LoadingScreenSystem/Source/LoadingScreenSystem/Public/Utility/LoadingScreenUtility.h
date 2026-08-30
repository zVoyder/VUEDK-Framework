// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LoadingScreenSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoadingScreenUtility.generated.h"

UCLASS()
class LOADINGSCREENSYSTEM_API ULoadingScreenUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<ULoadingScreenSubsystem> LoadingScreenSubsystem;
	
public:
	static void Init(ULoadingScreenSubsystem* Subsystem);

	UFUNCTION(BlueprintCallable, Category = "Loading Screen System")
	static void SetupLoadingScreen(const ULoadingScreenData* LoadingScreenData, const bool bAutoResetToDefault = true);

	UFUNCTION(BlueprintCallable, Category = "Loading Screen System")
	static void ResetToDefaultLoadingScreen();

	UFUNCTION(BlueprintCallable, Category = "Loading Screen System")
	static void ClearLoadingScreen(const bool bAutoResetToDefault = true);

private:
	static bool Check();
};
