// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotValidatorBase.h"
#include "CombatSlotValidator_Distance.generated.h"

UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotValidator_Distance : public UCombatSlotValidatorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validator")
	float MaxDistance = 1000.f;
	
protected:
	virtual bool IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const override;
};
