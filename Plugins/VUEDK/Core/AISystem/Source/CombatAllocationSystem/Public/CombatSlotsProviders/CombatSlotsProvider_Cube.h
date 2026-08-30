// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotsProviderBase.h"
#include "CombatSlotsProvider_Cube.generated.h"

UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotsProvider_Cube : public UCombatSlotsProviderBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Generation")
	float HalfGridSize = 1200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Generation")
	float SlotSpacing = 600.f;
	
public:
	UCombatSlotsProvider_Cube();

	virtual void ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation) override;
};
