// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CombatAllocatorObjectBase.h"
#include "CombatSlotEvaluatorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class COMBATALLOCATIONSYSTEM_API UCombatSlotEvaluatorBase : public UCombatAllocatorObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evaluation")
	float ScoreMultiplier = 1.f;
	
public:
	float Evaluate(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	float EvaluateSlot(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const;
};
