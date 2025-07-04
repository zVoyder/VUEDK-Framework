// Copyright VUDNK, Inc. All Rights Reserved.

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
	static ADismemberedLimb* SpawnDismemberedLimb(UDismemberer* Dismemberer, const FDismemberableLimbData& LimbData, const FVector& Impulse, float LifeSpan, float ScaleFactor = 1.f);
	
	static void SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FBloodDecalSpawnParams& BloodDecalSpawnParams);

	static UNiagaraComponent* SpawnDismemberExplosionFX(const UWorld* World, UBloodParticlesNiagaraCallbackHandler* CallbackHandler, UNiagaraSystem* ExplosionFX, const FVector& ExplosionLocation);

	static UNiagaraComponent* SpawnBloodSpillFX(const UWorld* World, UBloodParticlesNiagaraCallbackHandler* CallbackHandler, UNiagaraSystem* BloodSpillFX, USkinnedMeshComponent* SkinnedMeshComponent, const FDismemberableLimbData& LimbData, const FVector& FromPointDir, const FVector& ToPointDir, const FRotator& RotationOffset, float Depth);

	static UBloodParticlesNiagaraCallbackHandler* CreateBloodParticlesNiagaraCallbackHandler(UWorld* World, UDismemberer* Dismemberer);
	
	static const UDismembererSystemSettings* GetDismembererSettings();
};
