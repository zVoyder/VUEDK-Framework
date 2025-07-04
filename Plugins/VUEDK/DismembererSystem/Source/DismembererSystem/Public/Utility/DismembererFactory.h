// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismembererSystemSettings.h"
#include "DismemberedLimb.h"
#include "Data/BloodDecalSpawnParams.h"
#include "Data/DismemberableLimbData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DismembererFactory.generated.h"

UCLASS()
class DISMEMBERERSYSTEM_API UDismembererFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Spawns a dismembered limb actor in the world.
	 * @param Dismemberer - The dismemberer instance that will handle the dismembered limb.
	 * @param LimbData - The data for the dismembered limb, including mesh, material, and other properties.
	 * @param Impulse - The impulse to apply to the dismembered limb upon spawning, simulating the force of dismemberment.
	 * @param LifeSpan - The lifespan of the dismembered limb actor before it is automatically destroyed.
	 * @param ScaleFactor - A scale factor to apply to the dismembered limb's size. Defaults to 1.f (no scaling).
	 * @return A pointer to the spawned ADismemberedLimb actor, or nullptr if spawning failed.
	 */
	static ADismemberedLimb* SpawnDismemberedLimb(UDismemberer* Dismemberer, const FDismemberableLimbData& LimbData, const FVector& Impulse, float LifeSpan, float ScaleFactor = 1.f);

	/**
	 * Spawns a blood decal in the world at the specified location.
	 * @param World - The world context in which to spawn the decal.
	 * @param DecalMaterial - The material to use for the blood decal.
	 * @param ParticleLocation - The location in the world where the blood decal should be spawned.
	 * @param BloodDecalSpawnParams - Parameters for spawning the blood decal, including size, rotation, and lifetime.
	 */
	static void SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FBloodDecalSpawnParams& BloodDecalSpawnParams);

	/**
	 * Spawns a dismember explosion effect using Niagara.
	 * @param World - The world context in which to spawn the explosion effect.
	 * @param CallbackHandler - The callback handler for the Niagara system, which can handle custom events and interactions.
	 * @param ExplosionFX - The Niagara system to use for the explosion effect.
	 * @param ExplosionLocation - The location in the world where the explosion effect should be spawned.
	 * @return A pointer to the spawned UNiagaraComponent, or nullptr if spawning failed.
	 */
	static UNiagaraComponent* SpawnDismemberExplosionFX(const UWorld* World, UBloodParticlesNiagaraCallbackHandler* CallbackHandler, UNiagaraSystem* ExplosionFX, const FVector& ExplosionLocation);

	/**
	 * Spawns a blood spill effect using Niagara.
	 * @param World - The world context in which to spawn the blood spill effect.
	 * @param CallbackHandler - The callback handler for the Niagara system, which can handle custom events and interactions.
	 * @param BloodSpillFX - The Niagara system to use for the blood spill effect.
	 * @param SkinnedMeshComponent - The skinned mesh component associated with the dismembered limb, used for spawning the effect.
	 * @param LimbData - The data for the dismembered limb, including mesh, material, and other properties.
	 * @param FromPointDir - The direction vector from which the blood spill originates.
	 * @param ToPointDir - The direction vector towards which the blood spill is directed.
	 * @param RotationOffset - An optional rotation offset to apply to the effect's orientation.
	 * @param Depth - The depth of the blood spill effect, which can affect its appearance and behavior.
	 * @return A pointer to the spawned UNiagaraComponent, or nullptr if spawning failed.
	 */
	static UNiagaraComponent* SpawnBloodSpillFX(const UWorld* World, UBloodParticlesNiagaraCallbackHandler* CallbackHandler, UNiagaraSystem* BloodSpillFX, USkinnedMeshComponent* SkinnedMeshComponent, const FDismemberableLimbData& LimbData, const FVector& FromPointDir, const FVector& ToPointDir, const FRotator& RotationOffset, float Depth);

	/**
	 * Creates a blood particles Niagara callback handler for managing blood particle effects.
	 * @param World - The world context in which the callback handler will operate.
	 * @param Dismemberer - The dismemberer instance that will handle the blood particle effects.
	 * @return A pointer to the created UBloodParticlesNiagaraCallbackHandler, or nullptr if creation failed.
	 */
	static UBloodParticlesNiagaraCallbackHandler* CreateBloodParticlesNiagaraCallbackHandler(UWorld* World, UDismemberer* Dismemberer);

	/**
	 * Retrieves the dismemberer system settings.
	 * @return A pointer to the UDismembererSystemSettings instance containing configuration settings for the dismemberer system.
	 */
	static const UDismembererSystemSettings* GetDismembererSettings();
};
