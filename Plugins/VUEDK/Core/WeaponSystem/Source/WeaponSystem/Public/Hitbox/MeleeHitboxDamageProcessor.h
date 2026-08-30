// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MeleeHitboxDamageProcessor.generated.h"

class AWeaponMelee;
class UMeleeHitbox;

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UMeleeHitboxDamageProcessor : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "Damage multiplier applied to the weapon damage, after the processing."))
	float DamageMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseCustomDamageType", Tooltip = "Custom damage type to apply when this hitbox hits an actor."))
	TSubclassOf<UDamageType> CustomDamageType;
	UPROPERTY()
	bool bUseCustomDamageType = false;
	
public:
	TSubclassOf<UDamageType> GetDamageType() const;
	
	float ProcessDamageNative(const float WeaponDamage, UMeleeHitbox* Hitbox, AWeaponMelee* WeaponMelee) const;
	
	UFUNCTION(BlueprintNativeEvent)
	float ProcessDamage(const float WeaponDamage, UMeleeHitbox* Hitbox, AWeaponMelee* WeaponMelee) const;
};
