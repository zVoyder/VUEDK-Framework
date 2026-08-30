// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/DynamicOperations/DynamicValueOperation.h"

float UDynamicValueOperation::GetDynamicValue(const float InBaseValue) const
{
	return CalculateValue(InBaseValue, GetAgentPawn(), GetAgentController(), GetAgentBlackboard());
}

float UDynamicValueOperation::CalculateValue_Implementation(const float Value, APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard) const
{
	return Value;
}

