// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EShootType : uint8
{
	Simultaneous UMETA(DisplayName = "Simultaneous"),
	Sequential UMETA(DisplayName = "Sequential")
};
