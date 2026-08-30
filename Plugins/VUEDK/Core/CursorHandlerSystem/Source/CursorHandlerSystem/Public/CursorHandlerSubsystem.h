// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CursorHandlerSystemSettings.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "CursorHandlerSubsystem.generated.h"

constexpr float SensScale = 100.f;

UCLASS(NotBlueprintable, BlueprintType)
class CURSORHANDLERSYSTEM_API UCursorHandlerSubsystem : public ULocalPlayerSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	UInputDeviceSubsystem* InputDeviceSubsystem;
	UPROPERTY()
	const UCursorHandlerSystemSettings* CursorHandlerSystemSettings;
	UPROPERTY()
	UUserWidget* GamepadCursorWidget;
	UPROPERTY()
	UUserWidget* MouseCursorWidget;
	bool bIsUsingGamepad;
	bool bIsTickEnabled;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Deinitializes the subsystem and cleans up resources.
	 */
	virtual void Deinitialize() override;

	/**
	 * @brief Called when the player controller changes.
	 * @param NewPlayerController The new player controller instance.
	 */
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Returns true if the subsystem is tickable.
	 * @return True if tickable, false otherwise.
	 */
	virtual bool IsTickable() const override;

	/**
	 * @brief Returns the stat Id for this tickable object.
	 * @return The stat Id.
	 */
	virtual TStatId GetStatId() const override;

private:
	/**
	 * @brief Moves the cursor using analog input (e.g., gamepad stick).
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	void MoveCursorWithAnalog(float DeltaTime) const;

	/**
	 * @brief Binds the check for gamepad usage.
	 */
	void BindCheckGamepad();

	/**
	 * @brief Unbinds the check for gamepad usage.
	 */
	void UnbindCheckGamepad();

	/**
	 * @brief Creates the cursor widgets for gamepad and mouse.
	 */
	void CreateCursorsWidgets();
	
	/**
	 * @brief Checks if the user is using a gamepad.
	 * @param UserId The platform user Id.
	 * @param DeviceId The input device Id.
	 */
	UFUNCTION()
	void CheckUsingGamepad(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	/**
	 * @brief Sets the gamepad cursor widget as active.
	 */
	void SetGamepadCursorWidget() const;
	
	/**
	 * @brief Sets the mouse cursor widget as active.
	 */
	void SetMouseCursorWidget() const;
	
	/**
	 * @brief Checks the internal state of the subsystem for consistency.
	 * @return True if the state is valid, false otherwise.
	 */
	bool Check() const;
};
