// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum EPreconditionsMode : uint8
{
	All UMETA(DisplayName = "All", ToolTip = "All preconditions must be met to equip the item."),
	Any UMETA(DisplayName = "Any", ToolTip = "At least one precondition must be met to equip the item."),
};
