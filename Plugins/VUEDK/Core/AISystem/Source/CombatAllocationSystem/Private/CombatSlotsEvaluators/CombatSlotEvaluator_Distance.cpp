// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsEvaluators/CombatSlotEvaluator_Distance.h"

float UCombatSlotEvaluator_Distance::EvaluateSlot_Implementation(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const
{
	if (!IsValid(Requester))
		return 0.f;

	const float Distance = FVector::Distance(Requester->GetActorLocation(), Slot.Location);
	return Distance;
}
