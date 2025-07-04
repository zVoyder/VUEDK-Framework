// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Thrower/Throwables/ThrowableBase.h"
#include "ThrowableGrenade.generated.h"

UCLASS()
class WEAPONSYSTEM_API AThrowableGrenade : public AThrowableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable|Grenade")
	float Damage = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable|Grenade")
	float ExplosionRadius = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable|Grenade")
	TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable|Grenade")
	bool bDoFullDamage = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable|Grenade")
	TEnumAsByte<ECollisionChannel> ExplosionChannel = ECC_Visibility;

public:
	AThrowableGrenade();

	/**
	 * Makes the grenade explode immediately, dealing damage to actors within the explosion radius.
	 */
	UFUNCTION(BlueprintCallable)
	void Explode();

protected:
	virtual void OnThrowableLifeSpanEnd_Implementation() override;

	/**
	 * Called when the grenade explodes.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnExplosion();
};
