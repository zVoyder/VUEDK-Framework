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
	/**
	 * Initializes the InputsHandlerUtility with the given InputsHandlerSubsystem.
	 * @param InInputsHandler The InputsHandlerSubsystem to use.
	 */
	static void Init(UInputsHandlerSubsystem* InInputsHandler);

	/**
	 * Returns the Enhanced Input User Settings.
	 * @return Pointer to the UEnhancedInputUserSettings.
	 */
	UFUNCTION(BlueprintPure)
	static UEnhancedInputUserSettings* GetEnhancedInputUserSettings();
	
	/**
	 * Sets the input mode to Gameplay.
	 */
	UFUNCTION(BlueprintCallable)
	static void SetModeGameplay();

	/**
	 * Sets the input mode to UI.
	 */
	UFUNCTION(BlueprintCallable)
	static void SetModeUI();

	/**
	 * Checks if the user is currently using a gamepad.
	 * @return True if using a gamepad, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	static bool IsUsingGamepad();

private:
	/**
	 * Checks the validity or state of the InputsHandlerUtility.
	 * @return True if valid, false otherwise.
	 */
	static bool Check();
};
