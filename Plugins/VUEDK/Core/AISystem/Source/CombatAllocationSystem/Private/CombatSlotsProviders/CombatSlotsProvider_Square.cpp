// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsProviders/CombatSlotsProvider_Square.h"

void UCombatSlotsProvider_Square::ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation)
{
	const int32 NumSlotsPerSide = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(NumSlots)));

	const int32 Row = SlotIndex / NumSlotsPerSide;
	const int32 Col = SlotIndex % NumSlotsPerSide;

	OutLocation.X = ProviderLocation.X + (Col - (NumSlotsPerSide - 1) / 2.f) * SlotSpacing;
	OutLocation.Y = ProviderLocation.Y + (Row - (NumSlotsPerSide - 1) / 2.f) * SlotSpacing;
	OutLocation.Z = ProviderLocation.Z;
}
