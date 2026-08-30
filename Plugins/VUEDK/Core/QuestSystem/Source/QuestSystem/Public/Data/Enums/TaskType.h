// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class ETaskType : uint8
{
	None UMETA(DisplayName = "None"),
	Single UMETA(DisplayName = "Single"),
	Count UMETA(DisplayName = "Count")
};
