// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	None UMETA(DisplayName = "None"),
	Active UMETA(DisplayName = "Active"),
	Inactive UMETA(DisplayName = "Inactive"),
	Completed UMETA(DisplayName = "Completed")
};
