// Copyright VUEDK, Inc. All Rights Reserved.

#include "SenseValidators/SenseValidator_Distance.h"
#include "SimplePerceptionSystem.h"

bool USenseValidator_Distance::ReceiveValidate_Implementation(AActor* Actor) const
{
	const AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
		return false;

	const FVector Location = GetSightLocation();
	const float Distance = FVector::Distance(Location, Actor->GetActorLocation());

#if !UE_BUILD_SHIPPING
	if (FSimplePerceptionSystemModule::CVarDebugPerceptionSystem.GetValueOnGameThread())
	{
		DrawDebugCircle(
			GetWorld(),
			Location,
			MaxDistance,
			64,
			FColor::Red,
			false,
			0.f,
			0,
			2.f,
			FVector(1, 0, 0),
			FVector(0, 1, 0),
			true
		);
	}
#endif
	
	return Distance <= MaxDistance;
}
