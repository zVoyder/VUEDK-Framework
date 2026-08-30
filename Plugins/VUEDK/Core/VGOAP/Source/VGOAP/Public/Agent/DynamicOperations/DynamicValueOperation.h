// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Agent/Base/GOAPObjectBase.h"
#include "DynamicValueOperation.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VGOAP_API UDynamicValueOperation : public UGOAPObjectBase
{
	GENERATED_BODY()

public:
	float GetDynamicValue(float InBaseValue) const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	float CalculateValue(const float Value, APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard) const;
};
