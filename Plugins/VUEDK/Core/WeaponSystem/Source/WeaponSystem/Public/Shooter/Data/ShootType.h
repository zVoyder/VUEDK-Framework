// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EShootType : uint8
{
	None UMETA(DisplayName = "None"),
	Simultaneous UMETA(DisplayName = "Simultaneous"),
	Sequential UMETA(DisplayName = "Sequential")
};
