// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotsProviderBase.h"
#include "CombatSlotsProvider_Circle.generated.h"

UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotsProvider_Circle : public UCombatSlotsProviderBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.f, UIMin = 0.f), Category = "Combat Slots Provider|Generation")
	float Radius = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 360.f, UIMax = 360.f), Category = "Combat Slots Provider|Generation")
	float Angle = 0.f;
	
protected:
	virtual void ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation) override;
};
