// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/DismembererFactory.h"
#include "BloodDecal.h"
#include "BloodDecalsPool.h"
#include "BloodParticlesNiagaraCallbackHandler.h"
#include "DismembererSystemSettings.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Data/DismemberableLimbData.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ADismemberedLimb* UDismembererFactory::SpawnDismemberedLimb(UDismemberer* Dismemberer, const FDismemberableLimbData& LimbData, const FVector& Impulse, const float LifeSpan, const float ScaleFactor)
{
	const UWorld* World = Dismemberer->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismembererFactory::SpawnDismemberedLimb: Dismemberer has not a valid World."));
		return nullptr;
	}

	ADismemberedLimb* DismemberedLimb = Dismemberer->GetWorld()->SpawnActor<ADismemberedLimb>();
	DismemberedLimb->Init(LimbData, Dismemberer, Impulse, LifeSpan);
	DismemberedLimb->SetActorScale3D(ScaleFactor * DismemberedLimb->GetActorScale3D());
	return DismemberedLimb;
}

void UDismembererFactory::SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FBloodDecalSpawnParams& BloodDecalSpawnParams)
{
	if (!IsValid(World) || !IsValid(DecalMaterial))
		return;

	if (FMath::RandRange(0.f, 1.f) > BloodDecalSpawnParams.SpawnChance)
		return;

	TArray<FHitResult> HitResults;
	const FCollisionQueryParams CollisionParams;
	const float SizeComp = FMath::RandRange(BloodDecalSpawnParams.MinSize, BloodDecalSpawnParams.MaxSize);
	const FVector Size = FVector(SizeComp);
	const float Duration = GetDismembererSettings()->DecalsDuration;
	const float FadeInTime = GetDismembererSettings()->DecalFadeInTime;
	const float FadeOutTime = GetDismembererSettings()->DecalFadeOutTime;
	if (World->SweepMultiByChannel(HitResults, ParticleLocation, ParticleLocation, FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(Size.X / 2.f), CollisionParams))
	{
		// Attach blood decal to the first pawn hit if any
		const FHitResult* PawnHitResult = HitResults.FindByPredicate(
			[](const FHitResult& HitResult)
			{
				return HitResult.GetComponent()->bReceivesDecals && HitResult.GetActor() && HitResult.GetActor()->IsA<APawn>();
			}
		);
		
		if (PawnHitResult != nullptr)
		{
			UGameplayStatics::SpawnDecalAttached(DecalMaterial, Size / 2.f, PawnHitResult->Component.Get(), PawnHitResult->BoneName, PawnHitResult->ImpactPoint, PawnHitResult->ImpactNormal.Rotation(), EAttachLocation::Type::KeepWorldPosition, Duration);
			return;
		}

		UBloodDecalsPool* BloodDecalsPool = World->GetSubsystem<UBloodDecalsPool>();
		if (BloodDecalsPool == nullptr)
		{
			UE_LOG(LogDismembermentSystem, Error, TEXT("UDismembererFactory::SpawnBloodDecal: Decal pool is not valid."));
			return;
		}

		ABloodDecal* BloodDecal = BloodDecalsPool->AcquireDecal();
		if (!IsValid(BloodDecal))
			return;
		
		// Calculate average normal of all hit results and then spawn a blood decal oriented to that normal
		FVector AverageNormal = FVector::ZeroVector;
		for (const FHitResult& HitResult : HitResults)
		{
			const ECollisionChannel ObjectType = HitResult.GetActor()->GetRootComponent()->GetCollisionObjectType();
			if (ObjectType != ECC_WorldStatic && ObjectType != ECC_WorldDynamic)
				continue;
			
			AverageNormal += HitResult.ImpactNormal;
		}

		AverageNormal /= HitResults.Num();
		AverageNormal.Normalize();

		BloodDecal->Init(DecalMaterial, AverageNormal, Size, Duration, FMath::RandRange(BloodDecalSpawnParams.MinSortOrder, BloodDecalSpawnParams.MaxSortOrder), FadeInTime, FadeOutTime);
		BloodDecal->SetActorLocation(ParticleLocation);
	}
}

UNiagaraComponent* UDismembererFactory::SpawnDismemberExplosionFX(const UWorld* World, UBloodParticlesNiagaraCallbackHandler* CallbackHandler, UNiagaraSystem* ExplosionFX, const FVector& ExplosionLocation)
{
	if (!IsValid(World) || !IsValid(ExplosionFX))
		return nullptr;

	FFXSystemSpawnParameters SpawnParams;
	SpawnParams.WorldContextObject = World;
	SpawnParams.SystemTemplate = ExplosionFX;
	SpawnParams.Location = ExplosionLocation;
	SpawnParams.PoolingMethod = EPSCPoolMethod::AutoRelease;
	SpawnParams.bAutoActivate = true;
	SpawnParams.bPreCullCheck = true;
	SpawnParams.LocationType = EAttachLocation::KeepWorldPosition;
	SpawnParams.bAutoDestroy = false;
	
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocationWithParams(SpawnParams);
	NiagaraComponent->SetVariableObject(CallbackHandler->GetNiagaraCallbackName(), CallbackHandler);
	return NiagaraComponent;
}

UNiagaraComponent* UDismembererFactory::SpawnBloodSpillFX(const UWorld* World, UBloodParticlesNiagaraCallbackHandler* CallbackHandler, UNiagaraSystem* BloodSpillFX, USkinnedMeshComponent* SkinnedMeshComponent, const FDismemberableLimbData& LimbData, const FVector& FromPointDir, const FVector& ToPointDir, const FRotator& RotationOffset, float Depth)
{
	if (!IsValid(World) || !IsValid(BloodSpillFX) || !IsValid(SkinnedMeshComponent))
		return nullptr;

	const FVector SpillDirection = (FromPointDir - ToPointDir).GetSafeNormal();
	FRotator SprayRotation = SpillDirection.Rotation();
	SprayRotation += RotationOffset;

	FFXSystemSpawnParameters SpawnParams;
	SpawnParams.WorldContextObject = World;
	SpawnParams.SystemTemplate = BloodSpillFX;
	SpawnParams.AttachToComponent = SkinnedMeshComponent;
	SpawnParams.AttachPointName = SkinnedMeshComponent->GetParentBone(LimbData.BoneName);
	SpawnParams.PoolingMethod = EPSCPoolMethod::AutoRelease;
	SpawnParams.bAutoActivate = true;
	SpawnParams.bPreCullCheck = true;
	SpawnParams.LocationType = EAttachLocation::SnapToTargetIncludingScale;
	SpawnParams.bAutoDestroy = false;
	
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
	FVector Location = SkinnedMeshComponent->GetSocketLocation(LimbData.BoneName);
	Location += -SpillDirection * Depth;
	NiagaraComponent->SetWorldRotation(SprayRotation);
	NiagaraComponent->SetWorldLocation(Location);
	NiagaraComponent->SetVariableObject(CallbackHandler->GetNiagaraCallbackName(), CallbackHandler);
	return NiagaraComponent;
}

UBloodParticlesNiagaraCallbackHandler* UDismembererFactory::CreateBloodParticlesNiagaraCallbackHandler(UWorld* World, UDismemberer* Dismemberer)
{
	if (!IsValid(World))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismembererFactory::CreateBloodParticlesNiagaraCallbackHandler: Invalid world."));
		return nullptr;
	}

	if (!IsValid(Dismemberer))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismembererFactory::CreateBloodParticlesNiagaraCallbackHandler: Invalid dismemberer."));
		return nullptr;
	}
	
	UBloodParticlesNiagaraCallbackHandler* CallbackHandler = NewObject<UBloodParticlesNiagaraCallbackHandler>(World);
	CallbackHandler->Init(Dismemberer);
	return CallbackHandler;
}

const UDismembererSystemSettings* UDismembererFactory::GetDismembererSettings()
{
	return GetDefault<UDismembererSystemSettings>();
}
