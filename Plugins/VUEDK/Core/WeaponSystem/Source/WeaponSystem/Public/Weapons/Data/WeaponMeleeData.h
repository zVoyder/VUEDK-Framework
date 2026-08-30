// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMeleeData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FWeaponMeleeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> DamageHitBoxChannel = ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "List of collision channels that, if hit during the attack, will interrupt or play the weapon's attack interrupt animation."))
	TArray<TEnumAsByte<ECollisionChannel>> AttackInterruptChannels =
	{
		ECC_Visibility,
		ECC_WorldStatic,
		ECC_WorldDynamic,
		ECC_PhysicsBody,
		ECC_Vehicle
	};
};
