// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatAllocatorObjectBase.h"
#include "Data/CombatSlot.h"
#include "UObject/Object.h"
#include "CombatSlotValidatorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class COMBATALLOCATIONSYSTEM_API UCombatSlotValidatorBase : public UCombatAllocatorObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Validator",
		meta = (ToolTip = "Value to match in order for the slot to be considered valid."))
	bool bValue = true;

public:
	bool ValidateSlot(const FCombatSlot& CombatSlot, const FVector& ProviderLocation) const;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	bool IsSlotValid(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const;
};
