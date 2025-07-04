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

	virtual void Deinitialize() override;

	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

private:
	void MoveCursorWithAnalog(float DeltaTime) const;

	void BindCheckGamepad();

	void UnbindCheckGamepad();

	void CreateCursorsWidgets();
	
	UFUNCTION()
	void CheckUsingGamepad(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	void SetGamepadCursorWidget() const;
	
	void SetMouseCursorWidget() const;
	
	bool Check() const;
};
