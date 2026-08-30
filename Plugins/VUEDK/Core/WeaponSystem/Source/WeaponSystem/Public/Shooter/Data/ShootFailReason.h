// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EShootFailReason : uint8
{
	Error UMETA(DisplayName = "Error"),
	CoolDown UMETA(DisplayName = "CoolDown"),
	NoAmmo UMETA(DisplayName = "NoAmmo"),
	Condition UMETA(DisplayName = "ShootCondition"),
	NoShootPoints UMETA(DisplayName = "NoShootPoints"),
	ShootMode UMETA(DisplayName = "ShootMode"),
	Disabled UMETA(DisplayName = "BehaviourDisabled")
};
