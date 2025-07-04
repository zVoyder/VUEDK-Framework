// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EHitZoneRedirectorMode : uint8
{
	None UMETA(DisplayName = "None"),
	CustomName UMETA(DisplayName = "Custom Name"),
	AttachedBoneName UMETA(DisplayName = "Attached Bone Name")
};
