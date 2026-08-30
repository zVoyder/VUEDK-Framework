// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(NotBlueprintType)
enum class EInputModeType : uint8
{
	InputModeNone UMETA(DisplayName = "None"),
	InputModeGameplay UMETA(DisplayName = "Gameplay"),
	InputModeUI UMETA(DisplayName = "UI")
};
