// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	None UMETA(DisplayName = "None"),
	Parallel UMETA(DisplayName = "Parallel"),
	Sequential UMETA(DisplayName = "Sequential")
};
