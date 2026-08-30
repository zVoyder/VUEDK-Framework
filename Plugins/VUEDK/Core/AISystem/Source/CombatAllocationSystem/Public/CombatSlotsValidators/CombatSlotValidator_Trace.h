// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatSlotValidatorBase.h"
#include "CombatSlotValidator_Trace.generated.h"

UENUM(BlueprintType)
enum ETraceDirection
{
	FromProviderToSlot,
	FromSlotToProvider
};

UCLASS()
class COMBATALLOCATIONSYSTEM_API UCombatSlotValidator_Trace : public UCombatSlotValidatorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Validator")
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Validator")
	TEnumAsByte<ETraceDirection> TraceDirection = ETraceDirection::FromProviderToSlot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Validator")
	FVector SlotOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Validator")
	FVector ProviderOffset = FVector::ZeroVector;

public:
	UCombatSlotValidator_Trace();
	
protected:
	virtual bool IsSlotValid_Implementation(const FVector& ProviderLocation, const FVector& SlotLocation, AActor* Occupant) const override;
};
