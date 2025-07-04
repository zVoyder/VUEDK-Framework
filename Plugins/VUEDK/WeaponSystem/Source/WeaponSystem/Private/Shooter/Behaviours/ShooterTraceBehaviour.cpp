// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShooterTraceBehaviour.h"
#include "Engine/DamageEvents.h"
#include "Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

void UShooterTraceBehaviour::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& DirectionToTargetSpreaded) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid."));
		return;
	}

	switch (ShootTraceMode)
	{
	case EShootTraceMode::CameraSightTrace:
		CameraSightTrace(World, ShootPoint);
		return;
	case EShootTraceMode::ShootPointTrace:
		ShootPointTrace(World, ShootPoint);
		return;
	case EShootTraceMode::TargetTrace:
		TargetTrace(World, ShootPoint->GetShootPointLocation(), DirectionToTarget);
	default:
		break;
	}
}

void UShooterTraceBehaviour::OnShootTrace_Implementation(UShootBarrel* Barrel, const FVector& ShootPointLocation, const FVector& TraceStartLocation, const FVector& EndLocation, const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const
{
}

void UShooterTraceBehaviour::CameraSightTrace(const UWorld* World, const UShootPoint* ShootPoint) const
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
		ShootPointTrace(World, ShootPoint);
		return;
	}

	const FVector ShootPointLocation = ShootPoint->GetShootPointLocation();
	const FVector WorldShootDirection = CameraRotation.RotateVector(ShootPoint->GetShootPointSpreadedRelativeDirection());
	FVector TraceStartPoint = CameraStartPoint;
	FVector TraceEndPoint = TraceStartPoint + WorldShootDirection * GetMaxRange();

	const bool bIsInLineOfSight = IsInLineOfSight(ShootPointLocation, CameraHitPoint);
	if (!bIsInLineOfSight)
	{
		TraceStartPoint = ShootPointLocation;
		TraceEndPoint = ShootPointLocation + ShootPoint->GetShootPointSpreadedDirection() * GetMaxRange();
	}

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, ShootPointLocation, CameraHitPoint, bIsInLineOfSight ? FColor::Green : FColor::Red, false, DebugTraceLineDuration, 0, 1.0f);
#endif

	LineTraceDamage(World, ShootPointLocation, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::ShootPointTrace(const UWorld* World, const UShootPoint* ShootPoint) const
{
	if (!IsValid(ShootPoint))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::ShootPointTrace: ShootPoint is not valid."));
		return;
	}

	const FVector ShootPointLocation = ShootPoint->GetShootPointLocation();
	const FVector ShootPointDirection = ShootPoint->GetShootPointSpreadedDirection();
	const FVector TraceStartPoint = ShootPointLocation;
	const FVector TraceEndPoint = TraceStartPoint + ShootPointDirection * GetMaxRange();

	LineTraceDamage(World, ShootPointLocation, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::TargetTrace(const UWorld* World, const FVector& ShootPointLocation, const FVector& DirectionToTarget) const
{
	const FVector TraceEndPoint = ShootPointLocation + DirectionToTarget * GetMaxRange();
	LineTraceDamage(World, ShootPointLocation, ShootPointLocation, TraceEndPoint);
}

void UShooterTraceBehaviour::LineTraceDamage(const UWorld* World, const FVector& ShootPointLocation, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const
{
	FVector EndLocation = TraceEndPoint;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Shooter->GetOwner());
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.bReturnPhysicalMaterial = true;

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, TraceStartPoint, TraceEndPoint, FColor::Purple, false, DebugTraceLineDuration, 0, 1.0f);
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
			UGameplayStatics::ApplyPointDamage(HitActor, GetDamage(), HitResult.ImpactNormal, HitResult, Shooter->GetOwner()->GetInstigatorController(), Shooter->GetOwner(), DamageTypeClass);

#if WITH_EDITORONLY_DATA
			if (bDrawDebugTraceLines)
				DrawDebugBox(World, HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, DebugTraceLineDuration, 0, 1.0f);
#endif
		}
	}

	OnShootTrace(GetShootBarrel(), ShootPointLocation, TraceStartPoint, EndLocation, HitResults, DamageHitResults);
}
