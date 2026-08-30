// Copyright VUEDK, Inc. All Rights Reserved.

#include "Sensors/Sensor_SphereTrace.h"
#include "SimplePerceptionSystem.h"
#include "Kismet/KismetSystemLibrary.h"

void USensor_SphereTrace::ReceiveProcess_Implementation(const float DeltaTime)
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	const AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
		return;

	TArray<FHitResult> OutHits;
	const FVector Location = GetSightLocation();
	UKismetSystemLibrary::SphereTraceMulti(
		World,
		Location,
		Location,
		Radius,
		TraceChannel,
		false,
		{},
#if UE_BUILD_SHIPPING
		EDrawDebugTrace::None,
#else
		FSimplePerceptionSystemModule::CVarDebugPerceptionSystem.GetValueOnGameThread() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
#endif
		OutHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.f
	);

	for (const FHitResult& Hit : OutHits)
	{
		const AActor* HitActor = Hit.GetActor();

		if (bUseFilter &&
			(!IsValid(HitActor) || !HitActor->GetClass()->IsChildOf(ActorClassFilter)))
			continue;

		SenseActor(Hit.GetActor());
	}
}
