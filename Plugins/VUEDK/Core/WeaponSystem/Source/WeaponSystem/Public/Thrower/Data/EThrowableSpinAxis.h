// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM()
enum EThrowableSpinAxis : uint8
{
	ThrowableSpinAxis_Up UMETA(DisplayName = "Up"),
	ThrowableSpinAxis_Down UMETA(DisplayName = "Down"),
	ThrowableSpinAxis_Left UMETA(DisplayName = "Left"),
	ThrowableSpinAxis_Right UMETA(DisplayName = "Right"),
	ThrowableSpinAxis_Forward UMETA(DisplayName = "Forward"),
	ThrowableSpinAxis_Backward UMETA(DisplayName = "Backward"),
	ThrowableSpinAxis_Custom UMETA(DisplayName = "Custom")
};
