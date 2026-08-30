// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsValidators/CombatSlotValidator_Unique.h"

bool UCombatSlotValidator_Unique::IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const
{
	if (!IsValid(CombatAllocator))
		return false;
	
	if (!IsValid(Occupant))
		return true;

	TArray<FGameplayTag> FoundProviders;
	if (!CombatAllocator->FindUsingProviders(Occupant, FoundProviders))
		return true;
	
	return FoundProviders.Contains(ProviderTag) && FoundProviders.Num() == 1;
}
