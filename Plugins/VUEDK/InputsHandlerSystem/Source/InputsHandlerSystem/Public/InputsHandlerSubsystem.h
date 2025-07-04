// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputCoreTypes.h"
#include "InputsHandlerSystemSettings.h"
#include "Data/InputModeType.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "InputsHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnGampleayMappingContextSet
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnUIMappingContextSet
);

UCLASS(NotBlueprintable, BlueprintType)
class INPUTSHANDLERSYSTEM_API UInputsHandlerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnGampleayMappingContextSet OnGameplayMappingContextSet;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnUIMappingContextSet OnUIMappingContextSet;

protected:
	UPROPERTY(BlueprintReadOnly)
	APlayerController* PlayerController;

private:
	UPROPERTY()
	TArray<UInputMappingContext*> CommonMappingContexts;
	UPROPERTY()
	TArray<UInputMappingContext*> GameplayMappingContexts;
	UPROPERTY()
	TArray<UInputMappingContext*> UIMappingContexts;
	UPROPERTY()
	TArray<UInputMappingContext*> ExcludedContexts;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	UPROPERTY()
	UInputDeviceSubsystem* InputDeviceSubsystem;
	UPROPERTY()
	const UInputsHandlerSystemSettings* InputsHandlerSystemSettings;
	EInputModeType CurrentInputModeType;
	bool bIsUsingGamepad;

public:
	UInputsHandlerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;

	/**
	 * Returns the normalized mouse position.
	 * @return Normalized mouse position as a FVector2D.
	 */
	UFUNCTION(BlueprintPure)
	FVector2D GetNormalizedMousePosition() const;

	/**
	 * Gets the Enhanced Input User Settings.
	 * @return Pointer to the UEnhancedInputUserSettings.
	 */
	UFUNCTION(BlueprintPure)
	UEnhancedInputUserSettings* GetEnhancedInputUserSettings() const;

	/**
	 * Adds a common mapping context.
	 * @param MappingContext The mapping context to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddCommonMappingContext(UInputMappingContext* MappingContext);

	/**
	 * Removes a common mapping context.
	 * @param MappingContext The mapping context to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveCommonMappingContext(UInputMappingContext* MappingContext);
	
	/**
	 * Adds a gameplay mapping context.
	 * @param MappingContext The mapping context to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddGameplayMappingContext(UInputMappingContext* MappingContext);

	/**
	 * Removes a gameplay mapping context.
	 * @param MappingContext The mapping context to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveGameplayMappingContext(UInputMappingContext* MappingContext);

	/**
	 * Adds a UI mapping context.
	 * @param MappingContext The mapping context to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddUIMappingContext(UInputMappingContext* MappingContext);

	/**
	 * Removes a UI mapping context.
	 * @param MappingContext The mapping context to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveUIMappingContext(UInputMappingContext* MappingContext);

	/**
	 * Adds an excluded mapping context.
	 * @param MappingContext The mapping context to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddExcludedMappingContext(UInputMappingContext* MappingContext);

	/**
	 * Removes an excluded mapping context.
	 * @param MappingContext The mapping context to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveExcludedMappingContext(UInputMappingContext* MappingContext);
	
	/**
	 * Sets the input mode to Gameplay.
	 */
	UFUNCTION(BlueprintCallable)
	void SetModeGameplay();
	
	/**
	 * Sets the input mode to UI.
	 */
	UFUNCTION(BlueprintCallable)
	void SetModeUI();

	/**
	 * Gets all mapping contexts.
	 * @return Array of all UInputMappingContext pointers.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetAllMappingContexts() const;

	/**
	 * Gets the common mapping contexts.
	 * @return Array of common UInputMappingContext pointers.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetCommonMappingContexts() const;
	
	/**
	 * Gets the gameplay mapping contexts.
	 * @return Array of gameplay UInputMappingContext pointers.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetGameplayMappingContexts() const;

	/**
	 * Gets the UI mapping contexts.
	 * @return Array of UI UInputMappingContext pointers.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetUIMappingContexts() const;
	
	/**
	 * Returns the gameplay input mapping context at the specified index.
	 * @param Index The index of the gameplay mapping context to retrieve (default: 0).
	 * @return Pointer to the UInputMappingContext.
	 */
	UFUNCTION(BlueprintPure)
	UInputMappingContext* GetGameplayMappingContext(const int32 Index = 0) const;

	/**
	 * Returns the UI input mapping context at the specified index.
	 * @param Index The index of the UI mapping context to retrieve (default: 0).
	 * @return Pointer to the UInputMappingContext.
	 */
	UInputMappingContext* GetUIMappingContext(const int32 Index = 0) const;

	/**
	 * Checks if the user is currently using a gamepad.
	 * @return True if using a gamepad, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsUsingGamepad() const;

	/**
	 * Checks if the current input mode is Gameplay.
	 * @return True if input mode is Gameplay, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsInputModeGameplay() const;

	/**
	 * Checks if the current input mode is UI.
	 * @return True if input mode is UI, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsInputModeUI() const;

private:
	/**
	 * Sets the input mode to Gameplay.
	 */
	void SetInputModeGameplay() const;
	
	/**
	 * Sets the input mode to UI.
	 */
	void SetInputModeUI() const;

	/**
	 * Adds common input contexts.
	 */
	void AddCommonContexts() const;
	
	/**
	 * Adds gameplay-specific input contexts.
	 */
	void AddGameplayContexts() const;

	/**
	 * Removes gameplay-specific input contexts.
	 */
	void RemoveGameplayContexts() const;

	/**
	 * Adds UI-specific input contexts with the given priority.
	 * @param Priority The priority for the UI contexts.
	 */
	void AddUIContexts(int32 Priority) const;

	/**
	 * Removes UI-specific input contexts.
	 */
	void RemoveUIContexts() const;

	/**
	 * Removes excluded input contexts.
	 */
	void RemoveExcludedContexts() const;
	
	/**
	 * Binds the check for gamepad usage.
	 */
	void BindCheckGamepad();

	/**
	 * Unbinds the check for gamepad usage.
	 */
	void UnbindCheckGamepad();

	/**
	 * Centers the mouse on the viewport.
	 */
	void CenterMouseOnViewport() const;

	/**
	 * Loads all input mapping contexts.
	 */
	void LoadMappingContexts();
	
	/**
	 * Callback for checking if a gamepad is being used.
	 * @param UserId The platform user ID.
	 * @param DeviceId The input device ID.
	 */
	UFUNCTION()
	void CheckUsingGamepad(const FPlatformUserId UserId, const FInputDeviceId DeviceId);
	
	/**
	 * Checks the validity or state of the InputsHandlerSubsystem.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;
};
