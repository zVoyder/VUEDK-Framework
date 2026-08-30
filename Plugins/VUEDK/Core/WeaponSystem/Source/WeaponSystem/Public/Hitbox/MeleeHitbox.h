// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeleeHitboxDamageProcessor.h"
#include "Components/SceneComponent.h"
#include "MeleeHitbox.generated.h"

class UMeleeHitboxTracerBase;
class AWeaponMelee;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UMeleeHitbox : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Hitbox")
	UMeleeHitboxTracerBase* HitboxTracer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Hitbox")
	UMeleeHitboxDamageProcessor* DamageProcessor;

private:
	UPROPERTY()
	AWeaponMelee* WeaponMelee;

public:
	UMeleeHitbox();

	virtual void OnRegister() override;

	void Init(AWeaponMelee* InWeaponMelee);

	UFUNCTION(BlueprintCallable)
	TArray<FHitResult> TraceHitbox() const;

	UFUNCTION(BlueprintPure)
	float GetProcessedDamage();

	UFUNCTION(BlueprintPure)
	TSubclassOf<UDamageType> GetDamageType() const;

	AWeaponMelee* GetWeaponMelee() const;

private:
	bool Check() const;
};
