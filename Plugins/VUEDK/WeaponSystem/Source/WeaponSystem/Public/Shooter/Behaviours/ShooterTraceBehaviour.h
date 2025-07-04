// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBehaviourBase.h"
#include "Shooter/Data/ShootTraceMode.h"
#include "ShooterTraceBehaviour.generated.h"

UCLASS()
class WEAPONSYSTEM_API UShooterTraceBehaviour : public UShooterBehaviourBase
{
	GENERATED_BODY()

public:
	// -- Trace --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"), Category = "Shooter|Trace")
	int32 MaxPenetration = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooter|Trace")
	bool bAllowsMultipleHitsOnSameActor = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Shooter|Trace")
	TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECollisionChannel::ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Shooter|Trace")
	TEnumAsByte<EShootTraceMode> ShootTraceMode = EShootTraceMode::CameraSightTrace;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugTraceLines = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bDrawDebugTraceLines", EditConditionHides))
	float DebugTraceLineDuration = 5.f;
#endif

protected:
	virtual void OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& DirectionToTargetSpreaded) const override;
	
	/**
	 * Called when the shoot trace is completed.
	 * @param Barrel The shoot barrel that performed the trace.
	 * @param ShootPointLocation The location of the shoot point.
	 * @param TraceStartLocation The start location of the trace.
	 * @param EndLocation The actual end location of the trace. If a hit is detected, this will be the impact point; otherwise, it will match the original trace end point.
	 * @param TraceHitResults The subset of hit results that were detected during the trace.
	 * @param DamageHitResults The subset of hit results that resulted in actual damage being applied to valid targets.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnShootTrace(UShootBarrel* Barrel, const FVector& ShootPointLocation, const FVector& TraceStartLocation, const FVector& EndLocation, const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const;

private:
	void CameraSightTrace(const UWorld* World, const UShootPoint* ShootPoint) const;

	void ShootPointTrace(const UWorld* World, const UShootPoint* ShootPoint) const;

	void TargetTrace(const UWorld* World, const FVector& ShootPointLocation, const FVector& DirectionToTarget) const;

	void LineTraceDamage(const UWorld* World, const FVector& ShootPointLocation, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const;
};
