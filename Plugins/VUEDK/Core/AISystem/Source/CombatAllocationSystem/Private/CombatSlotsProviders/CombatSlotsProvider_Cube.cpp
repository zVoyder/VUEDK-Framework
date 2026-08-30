// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsProviders/CombatSlotsProvider_Cube.h"

UCombatSlotsProvider_Cube::UCombatSlotsProvider_Cube()
{
	bProjectToNavMesh = false;
}

void UCombatSlotsProvider_Cube::ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation)
{
	const int32 NumSlotsPerAxis = FMath::CeilToInt(FMath::Pow(static_cast<float>(NumSlots), 1.f / 3.f));
	const int32 SlotsPerLayer = NumSlotsPerAxis * NumSlotsPerAxis;

	const int32 Layer = SlotIndex / SlotsPerLayer;
	const int32 LayerRemainder = SlotIndex % SlotsPerLayer;
	const int32 Row = LayerRemainder / NumSlotsPerAxis;
	const int32 Col = LayerRemainder % NumSlotsPerAxis;

	const float GridExtent = (NumSlotsPerAxis > 1)
		? FMath::Min(SlotSpacing, (HalfGridSize * 2.f) / (NumSlotsPerAxis - 1))
		: 0.f;
	const float HalfGridOffset = (NumSlotsPerAxis - 1) / 2.f;

	OutLocation = ProviderLocation + FVector(
		(Col - HalfGridOffset) * GridExtent,
		(Row - HalfGridOffset) * GridExtent,
		(Layer - HalfGridOffset) * GridExtent);
}
