// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/StatOperation.h"

void UStatOperation::Init(UObject* InPayload, const float InModifierValue)
{
	Payload = InPayload;
	ModifierValue = InModifierValue;
	OnInit();
}

void UStatOperation::OnInit_Implementation()
{
}

float UStatOperation::GetResultOperation_Implementation() const
{
	return ModifierValue / 2;
}
