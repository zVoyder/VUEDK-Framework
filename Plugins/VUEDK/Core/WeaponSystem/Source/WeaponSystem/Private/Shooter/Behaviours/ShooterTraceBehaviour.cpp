// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShooterTraceBehaviour.h"
#include "WeaponSystem.h"
#include "Engine/DamageEvents.h"
#include "Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

void UShooterTraceBehaviour::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& ShootDirection) const
{
	switch (ShootTraceMode)
	{
	case EShootTraceMode::DefaultTrace:
		DefaultShootTrace(ShootPoint, ShootDirection);
		return;
	case EShootTraceMode::CameraSightTrace:
		CameraSightShootTrace(ShootPoint, ShootDirection);
	default:
		break;
	}
}

void UShooterTraceBehaviour::OnShootTrace_Implementation(UShootBarrel* Barrel, const FVector& ShootPointLocation, const FVector& TraceStartLocation, const FVector& EndLocation, const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const
{
}

void UShooterTraceBehaviour::DefaultShootTrace(const UShootPoint* ShootPoint, const FVector& ShootDirection) const
{
	const FVector TraceEndPoint = ShootDirection + ShootDirection * GetMaxRange();
	LineTraceDamage(ShootPoint->GetShootPointLocation(), ShootDirection, TraceEndPoint);
}

void UShooterTraceBehaviour::CameraSightShootTrace(const UShootPoint* ShootPoint, const FVector& ShootDirection) const
{
	if (!IsValid(ShootPoint))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::CameraOriginTrace: ShootPoint is not valid."));
		return;
	}

	FVector CameraStartPoint;
	FVector CameraHitPoint;
	FVector CameraEndPoint;
	FRotator CameraRotation;
	if (!TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint, CameraRotation, ShootPoint->GetShootPointRelativeLocation()))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::CameraOriginTrace: TryGetCameraPoints failed, using Fallback option."));
		DefaultShootTrace(ShootPoint, ShootDirection); // Fallback to default trace if camera points cannot be obtained
		return;
	}

	const FVector ShootPointLocation = ShootPoint->GetShootPointLocation();
	const FVector WorldShootDirection = CameraRotation.RotateVector(ShootPoint->GetShootPointSpreadedRelativeDirection());
	const bool bIsInLineOfSight = IsInLineOfSight(ShootPointLocation, CameraHitPoint);
	const FVector TraceStartPoint = bIsInLineOfSight ? CameraStartPoint : ShootPointLocation;
	const FVector TraceEndPoint = TraceStartPoint + WorldShootDirection * GetMaxRange();

#if !UE_BUILD_SHIPPING
	const bool bDebug = FWeaponSystemModule::CVarShootTraceDebug.GetValueOnGameThread();
	const float DebugTraceDuration = FWeaponSystemModule::CVarShootTraceDebugDuration.GetValueOnGameThread();
	
	if (bDebug)
		DrawDebugLine(GetWorld(), ShootPointLocation, CameraHitPoint, bIsInLineOfSight ? FColor::Green : FColor::Red, false, DebugTraceDuration, 0, 1.0f);
#endif

	LineTraceDamage(ShootPointLocation, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::LineTraceDamage(const FVector& ShootPointLocation, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
		return;
	
	FVector EndLocation = TraceEndPoint;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Shooter->GetOwner());
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.bReturnPhysicalMaterial = true;

#if !UE_BUILD_SHIPPING
	const bool bDebug = FWeaponSystemModule::CVarShootTraceDebug.GetValueOnGameThread();
	const float DebugTraceDuration = FWeaponSystemModule::CVarShootTraceDebugDuration.GetValueOnGameThread();
	
	if (bDebug)
		DrawDebugLine(World, TraceStartPoint, TraceEndPoint, FColor::Purple, false, DebugTraceDuration, 0, 1.0f);
#endif

	TArray<FHitResult> HitResults;
	TArray<FHitResult> DamageHitResults;
	World->LineTraceMultiByChannel(HitResults, TraceStartPoint, TraceEndPoint, DamageTraceChannel, CollisionQueryParams);

	if (HitResults.Num() > 0)
	{
		TArray<AActor*> HitActors;
		int32 PenetrationCount = 0;
		EndLocation = HitResults[0].ImpactPoint;

		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (!IsValid(HitActor))
				continue;

			if (PenetrationCount >= MaxPenetration)
				break;

			if (!bAllowsMultipleHitsOnSameActor && HitActors.Contains(HitActor))
				continue;
			
			HitActors.AddUnique(HitActor);
			DamageHitResults.Add(HitResult);
			PenetrationCount++;
			FDamageEvent DamageEvent;
			DamageEvent.DamageTypeClass = UDamageType::StaticClass();
			UGameplayStatics::ApplyPointDamage(HitActor, GetDamage(), HitResult.ImpactNormal, HitResult, Shooter->GetOwner()->GetInstigatorController(), Shooter->GetOwner(), GetDamageTypeClass());

#if !UE_BUILD_SHIPPING
			if (bDebug)
				DrawDebugBox(World, HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, DebugTraceDuration, 0, 1.0f);
#endif
		}
	}

	OnShootTrace(GetShootBarrel(), ShootPointLocation, TraceStartPoint, EndLocation, HitResults, DamageHitResults);
}
