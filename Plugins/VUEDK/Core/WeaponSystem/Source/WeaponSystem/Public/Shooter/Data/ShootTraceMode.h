// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum EShootTraceMode : uint8
{
	DefaultTrace UMETA(DisplayName = "Default Trace", ToolTip = "Default trace using shoot point direction."),
	CameraSightTrace UMETA(DisplayName = "Camera Sight Trace", ToolTip = "Trace that starts from the camera and checks if the shoot point is in line of sight."),
};
