// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "Weapons/WeaponBase.h"
#include "WeaponAnimMetaData.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponAnimMetaData : public UAnimMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AWeaponBase* Weapon;

public:
	void Init(AWeaponBase* InWeapon);
};
