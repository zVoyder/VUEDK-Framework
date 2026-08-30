// Copyright VUEDK, Inc. All Rights Reserved.

#include "SenseValidators/SenseValidator_Dot.h"
#include "SimplePerceptionSystem.h"

bool USenseValidator_Dot::ReceiveValidate_Implementation(AActor* Actor) const
{
	const AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
		return false;

	const FVector SightLocation = GetSightLocation();
	const bool Valid =
		FVector::DotProduct(OwnerActor->GetActorForwardVector(), (Actor->GetActorLocation() - SightLocation).GetSafeNormal())
		>= FMath::Cos(FMath::DegreesToRadians(DotThreshold));

#if !UE_BUILD_SHIPPING
	if (FSimplePerceptionSystemModule::CVarDebugPerceptionSystem.GetValueOnGameThread())
	{
		DrawDebugCone(
			GetWorld(),
			SightLocation,
			OwnerActor->GetActorForwardVector(),
			1000.f,
			FMath::DegreesToRadians(DotThreshold),
			FMath::DegreesToRadians(DotThreshold),
			12,
			Valid ? FColor::Green : FColor::Red,
			false,
			0.f,
			0,
			2.f
		);
	}
#endif

	return Valid;
}
