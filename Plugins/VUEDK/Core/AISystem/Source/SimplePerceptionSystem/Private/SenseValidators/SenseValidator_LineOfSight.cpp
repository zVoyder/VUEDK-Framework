// Copyright VUEDK, Inc. All Rights Reserved.

#include "SenseValidators/SenseValidator_LineOfSight.h"
#include "SimplePerceptionSystem.h"
#include "Kismet/KismetSystemLibrary.h"

bool USenseValidator_LineOfSight::ReceiveValidate_Implementation(AActor* Actor) const
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
		return false;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Actor);
	ActorsToIgnore.Add(OwnerActor);
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetSightLocation(),
		Actor->GetActorLocation(),
		TraceChannel,
		false,
		ActorsToIgnore,
#if UE_BUILD_SHIPPING
		EDrawDebugTrace::None,
#else
		FSimplePerceptionSystemModule::CVarDebugPerceptionSystem.GetValueOnGameThread() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
#endif
		HitResult,
		true
	);

	return !HitResult.bBlockingHit;
}
