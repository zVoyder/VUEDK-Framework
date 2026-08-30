// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsValidators/Base/CombatSlotValidatorBase.h"

bool UCombatSlotValidatorBase::ValidateSlot(const FCombatSlot& CombatSlot, const FVector& ProviderLocation) const
{
	return IsSlotValid(ProviderLocation, CombatSlot.Location, CombatSlot.Occupant.Get()) == bValue;
}

bool UCombatSlotValidatorBase::IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const
{
	return true;
}
