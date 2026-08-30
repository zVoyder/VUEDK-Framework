// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsEvaluators/Base/CombatSlotEvaluatorBase.h"

float UCombatSlotEvaluatorBase::Evaluate(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const
{
	return EvaluateSlot(Slot, ProviderLocation, Requester) * ScoreMultiplier;
}

float UCombatSlotEvaluatorBase::EvaluateSlot_Implementation(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const
{
	return 0.f;
}
