// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/WeaponAnimFactory.h"

UWeaponAnimMetaData* UWeaponAnimFactory::CreateWeaponAnimMetaData(AWeaponBase* InWeapon)
{
	if (!InWeapon)
		return nullptr;

	UWeaponAnimMetaData* AnimMetaData = NewObject<UWeaponAnimMetaData>(InWeapon);
	AnimMetaData->Init(InWeapon);
	return AnimMetaData;
}
