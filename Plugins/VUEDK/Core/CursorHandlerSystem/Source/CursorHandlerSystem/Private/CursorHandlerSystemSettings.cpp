// Copyright VUEDK, Inc. All Rights Reserved.

#include "CursorHandlerSystemSettings.h"

UCursorHandlerSystemSettings::UCursorHandlerSystemSettings(const FObjectInitializer& ObjectInitializer) : GamepadCursorWidget(nullptr),
                                                                                                          MouseCursorWidget(nullptr),
                                                                                                          CursorStick(EControllerAnalogStick::CAS_LeftStick),
                                                                                                          AnalogSensitivity(10.f),
                                                                                                          AnalogDeadzone(.2f)
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
