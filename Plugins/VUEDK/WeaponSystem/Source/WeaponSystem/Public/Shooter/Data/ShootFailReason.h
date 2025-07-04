// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EShootFailReason : uint8
{
	Error UMETA(DisplayName = "Error"),
	CoolDown UMETA(DisplayName = "CoolDown"),
	NoAmmo UMETA(DisplayName = "NoAmmo"),
	Condition UMETA(DisplayName = "Condition"),
	NoShootPoints UMETA(DisplayName = "NoShootPoints"),
	Disabled UMETA(DisplayName = "BehaviourDisabled")
};
