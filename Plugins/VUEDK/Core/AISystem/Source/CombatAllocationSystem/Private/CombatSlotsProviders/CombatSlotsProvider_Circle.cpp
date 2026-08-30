// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsProviders/CombatSlotsProvider_Circle.h"

void UCombatSlotsProvider_Circle::ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation)
{
	const float AngleStep = 360.f / NumSlots;
	const float AngleDeg = (AngleStep * SlotIndex) + Angle;
	const float AngleRad = FMath::DegreesToRadians(AngleDeg);

	const FVector Offset(
		FMath::Cos(AngleRad) * Radius,
		FMath::Sin(AngleRad) * Radius,
		0.f);

	OutLocation = ProviderLocation + Offset;
}
