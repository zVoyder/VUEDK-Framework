// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotEvaluatorBase.h"
#include "CombatSlotEvaluator_Distance.generated.h"

UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotEvaluator_Distance : public UCombatSlotEvaluatorBase
{
	GENERATED_BODY()

protected:
	virtual float EvaluateSlot_Implementation(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const override;
};
