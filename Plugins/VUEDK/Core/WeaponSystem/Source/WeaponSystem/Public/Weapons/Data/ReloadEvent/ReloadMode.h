// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EReloadMode : uint8
{
	Parallel UMETA(DisplayName = "Parallel", ToolTip = "All magazines are reloaded at the same time with the first behaviour reload montage"),
	Sequential UMETA(DisplayName = "Sequential", ToolTip = "All magazines are reloaded one by one with each behaviour reload montage")
};
