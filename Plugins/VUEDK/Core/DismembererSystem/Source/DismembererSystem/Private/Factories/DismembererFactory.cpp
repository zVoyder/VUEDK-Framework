// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/DismembererFactory.h"
#include "DismembererSystem.h"
#include "Effects/BloodDecal.h"
#include "Effects/BloodDecalsPool.h"
#include "Effects/BloodParticlesNiagaraCallbackHandler.h"
#include "DismemberLimbOperation.h"
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

	const UDismembererSystemSettings* Settings = GetDismembererSettings();
	if (!IsValid(Settings))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismembererFactory::SpawnDismemberedLimb: Dismemberer settings is not valid."));
		return nullptr;
	}

	UDismemberLimbOperation* LimbOperation = UDismembererFactory::CreateDismemberLimbOperation(Dismemberer);
	if (!IsValid(LimbOperation))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismembererFactory::SpawnDismemberedLimb: Dismember limb operation is not valid."));
		return nullptr;
	}
	
	ADismemberedLimb* DismemberedLimb = LimbOperation->SpawnLimbActor(Settings->DismemberedLimb.LoadSynchronous());
	if (!IsValid(DismemberedLimb))
		return nullptr;

	DismemberedLimb->Init(LimbData, Dismemberer, Impulse, LifeSpan);
	DismemberedLimb->SetActorScale3D(ScaleFactor * DismemberedLimb->GetActorScale3D());
	return DismemberedLimb;
}

void UDismembererFactory::SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FBloodDecalSpawnParams& BloodDecalSpawnParams)
{
	if (!IsValid(World) || !IsValid(DecalMaterial))
		return;

	if (FMath::FRand() > BloodDecalSpawnParams.SpawnChance)
		return;

	const UDismembererSystemSettings* Settings = GetDismembererSettings();
	if (!IsValid(Settings))
		return;

	const float SizeComp = FMath::FRandRange(BloodDecalSpawnParams.MinSize, BloodDecalSpawnParams.MaxSize);
	const FVector DecalSize(SizeComp);
	const float SphereRadius = SizeComp * 2.f;

	TArray<FHitResult> HitResults;
	const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BloodDecalSweep), false);

	if (!World->SweepMultiByChannel(
		HitResults,
		ParticleLocation,
		ParticleLocation,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(SphereRadius),
		QueryParams))
	{
		return;
	}

	for (const FHitResult& Hit : HitResults)
	{
		const UPrimitiveComponent* Comp = Hit.GetComponent();
		const AActor* Actor = Hit.GetActor();

		if (!Comp || !Actor || !Comp->bReceivesDecals)
			continue;

		if (Actor->IsA<APawn>())
		{
			UGameplayStatics::SpawnDecalAttached(
				DecalMaterial,
				DecalSize * 0.5f,
				Hit.Component.Get(),
				Hit.BoneName,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation(),
				EAttachLocation::KeepWorldPosition,
				Settings->DecalsDuration);
			return;
		}
	}

	const float CosMaxAngle = FMath::Cos(FMath::DegreesToRadians(25.0f));
	const FHitResult* BestHit = nullptr;
	FVector BestNormal = FVector::ZeroVector;
	int32 ValidHits = 0;

	for (const FHitResult& Hit : HitResults)
	{
		const AActor* Actor = Hit.GetActor();
		const UPrimitiveComponent* Comp = Hit.GetComponent();
		if (!Actor || !Comp || !Comp->bReceivesDecals)
			continue;

		const USceneComponent* RootComp = Actor->GetRootComponent();
		if (!RootComp)
			continue;

		const ECollisionChannel ObjType = RootComp->GetCollisionObjectType();
		if (ObjType != ECC_WorldStatic && ObjType != ECC_WorldDynamic)
			continue;

		const FVector N = Hit.ImpactNormal.GetSafeNormal();
		if (N.IsNearlyZero())
			continue;

		if (ValidHits == 0)
		{
			BestHit = &Hit;
			BestNormal = N;
			ValidHits = 1;
			continue;
		}
		
		const float Dot = FVector::DotProduct(BestNormal, N);
		if (Dot < CosMaxAngle)
			return;

		const float CurrDistSq = FVector::DistSquared(ParticleLocation, Hit.ImpactPoint);
		const float BestDistSq = FVector::DistSquared(ParticleLocation, BestHit->ImpactPoint);
		if (CurrDistSq < BestDistSq)
		{
			BestHit = &Hit;
			BestNormal = N;
		}

		++ValidHits;
	}

	if (!BestHit)
		return;

	UBloodDecalsPool* BloodDecalsPool = World->GetSubsystem<UBloodDecalsPool>();
	if (!BloodDecalsPool)
		return;

	ABloodDecal* BloodDecal = BloodDecalsPool->AcquireDecal();
	if (!IsValid(BloodDecal))
		return;

	const int32 SortOrder = FMath::RandRange(BloodDecalSpawnParams.MinSortOrder, BloodDecalSpawnParams.MaxSortOrder);

	BloodDecal->Init(
		DecalMaterial,
		BestNormal,
		DecalSize,
		Settings->DecalsDuration,
		SortOrder,
		Settings->DecalFadeInTime,
		Settings->DecalFadeOutTime);

	BloodDecal->SetActorLocation(BestHit->ImpactPoint + BestNormal * 0.5f);
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

UDismemberLimbOperation* UDismembererFactory::CreateDismemberLimbOperation(UDismemberer* Dismemberer)
{
	const UDismembererSystemSettings* Settings = GetDismembererSettings();
	if (!IsValid(Settings))
		return nullptr;
	
	UDismemberLimbOperation* Operation = NewObject<UDismemberLimbOperation>(Dismemberer, Settings->DismemberLimbOperation.LoadSynchronous());
	Operation->Init(Dismemberer);
	return Operation;
}
