// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsValidators/CombatSlotValidator_Trace.h"
#include "CombatAllocationSystem.h"
#include "Kismet/KismetSystemLibrary.h"

UCombatSlotValidator_Trace::UCombatSlotValidator_Trace()
{
	bValue = false;
}

bool UCombatSlotValidator_Trace::IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Occupant);
	FHitResult Hit;

	const FVector SlotLocationOffset = SlotLocation + SlotOffset;
	const FVector ProviderLocationOffset = ProviderLocation + ProviderOffset;
	const FVector TraceStart = TraceDirection == ETraceDirection::FromProviderToSlot ? ProviderLocationOffset : SlotLocationOffset;
	const FVector TraceEnd = TraceDirection == ETraceDirection::FromProviderToSlot ? SlotLocationOffset : ProviderLocationOffset;
	const EDrawDebugTrace::Type DebugType = FCombatAllocationSystemModule::CVarDrawDebugCombatSlots.GetValueOnGameThread() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	return UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, TraceChannel, false, ActorsToIgnore, DebugType, Hit, true);
}
