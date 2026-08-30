// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/MeleeHitboxDamageProcessor.h"

TSubclassOf<UDamageType> UMeleeHitboxDamageProcessor::GetDamageType() const
{
	if (bUseCustomDamageType)
		return CustomDamageType;
	
	return UDamageType::StaticClass();
}

float UMeleeHitboxDamageProcessor::ProcessDamageNative(const float WeaponDamage, UMeleeHitbox* Hitbox, AWeaponMelee* WeaponMelee) const
{
	return ProcessDamage(WeaponDamage, Hitbox, WeaponMelee) * DamageMultiplier;
}

float UMeleeHitboxDamageProcessor::ProcessDamage_Implementation(const float WeaponDamage, UMeleeHitbox* Hitbox, AWeaponMelee* WeaponMelee) const
{
	return WeaponDamage;
}

