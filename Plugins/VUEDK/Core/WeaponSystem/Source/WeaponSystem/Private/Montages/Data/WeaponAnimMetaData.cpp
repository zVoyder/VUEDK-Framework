// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Data/WeaponAnimMetaData.h"

void UWeaponAnimMetaData::Init(AWeaponBase* InWeapon)
{
	if (!InWeapon)
		return;

	Weapon = InWeapon;
}
