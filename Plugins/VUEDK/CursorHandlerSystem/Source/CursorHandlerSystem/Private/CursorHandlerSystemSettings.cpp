// Copyright VUEDK, Inc. All Rights Reserved.

#include "CursorHandlerSystemSettings.h"

UCursorHandlerSystemSettings::UCursorHandlerSystemSettings(const FObjectInitializer& ObjectInitializer): StickCursorSensitivity(10.f),
                                                                                                         GamepadCursorWidget(nullptr),
                                                                                                         MouseCursorWidget(nullptr),
                                                                                                         CursorStick(EControllerAnalogStick::CAS_LeftStick)
{
}

TSubclassOf<UUserWidget> UCursorHandlerSystemSettings::GetGamepadCursorClass() const
{
	return GamepadCursorWidget.LoadSynchronous();
}

TSubclassOf<UUserWidget> UCursorHandlerSystemSettings::GetMouseCursorClass() const
{
	return MouseCursorWidget.LoadSynchronous();
}
