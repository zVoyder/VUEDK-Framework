// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotValidatorBase.h"
#include "CombatSlotValidator_Unique.generated.h"

/**
 * @brief Validates that this is the only provider used by the occupant, meaning that if the occupant has a slot in any provider, this slot will be considered invalid.
 */
UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotValidator_Unique : public UCombatSlotValidatorBase
{
	GENERATED_BODY()

protected:
	virtual bool IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const override;
};
