// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotsProviderBase.h"
#include "CombatSlotsProvider_Square.generated.h"

UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotsProvider_Square : public UCombatSlotsProviderBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Generation")
	float SlotSpacing = 100.f;
	
protected:
	virtual void ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation) override;
};
