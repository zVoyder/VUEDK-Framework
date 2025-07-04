// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class ESaveTypeFilter : uint8
{
	All UMETA(DisplayName = "All"),
	Manual UMETA(DisplayName = "Manual"),
	Auto UMETA(DisplayName = "Auto"),
};
