// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsValidators/CombatSlotValidator_Distance.h"

bool UCombatSlotValidator_Distance::IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const
{
	if (!IsValid(Occupant))
		return true;
	
	const FVector Location = Occupant->GetActorLocation();
	return FVector::Dist(ProviderLocation, Location) <= MaxDistance;
}
