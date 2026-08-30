// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EQualityLevel : uint8
{
	Low UMETA(DisplayName = "Low", ToolTip = "Low quality."),
	Medium UMETA(DisplayName = "Medium", ToolTip = "Medium quality."),
	High UMETA(DisplayName = "High", ToolTip = "High quality."),
	Epic UMETA(DisplayName = "Epic", ToolTip = "Epic quality."),
	Cinematic UMETA(DisplayName = "Cinematic", ToolTip = "Cinematic quality."),
};
