// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EContextsBehaviour : uint8
{
	Switch UMETA(DisplayName = "Switch", ToolTip = "Remove the gameplay contexts and add the UI context, disabling the gameplay inputs."),
	Priority UMETA(DisplayName = "Priority", ToolTip = "Add the UI contexts with a priority, keeping the gameplay inputs active."),
};
