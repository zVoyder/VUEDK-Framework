// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Blueprint/UserWidget.h"
#include "CursorHandlerSystemSettings.generated.h"

UCLASS(Config = CursorHandlerSystemSettings, DefaultConfig, meta = (DisplayName = "Cursor Handler System Settings"))
class CURSORHANDLERSYSTEM_API UCursorHandlerSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSoftClassPtr<UUserWidget> GamepadCursorWidget;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSoftClassPtr<UUserWidget> MouseCursorWidget;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TEnumAsByte<EControllerAnalogStick::Type> CursorStick;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	float AnalogSensitivity;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	float AnalogDeadzone;

public:
	explicit UCursorHandlerSystemSettings(const FObjectInitializer& ObjectInitializer);

	TSubclassOf<UUserWidget> GetGamepadCursorClass() const;

	TSubclassOf<UUserWidget> GetMouseCursorClass() const;
};
