// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputsHandlerSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputsHandlerUtility.generated.h"

UCLASS()
class INPUTSHANDLERSYSTEM_API UInputsHandlerUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UInputsHandlerSubsystem* InputsHandlerSubsystem;

public:
	static void Init(UInputsHandlerSubsystem* InInputsHandler);

	UFUNCTION(BlueprintPure)
	static UEnhancedInputUserSettings* GetEnhancedInputUserSettings();
	
	UFUNCTION(BlueprintCallable)
	static void SetModeGameplay();

	UFUNCTION(BlueprintCallable)
	static void SetModeUI();

	UFUNCTION(BlueprintPure)
	static bool IsUsingGamepad();

private:
	static bool Check();
};
