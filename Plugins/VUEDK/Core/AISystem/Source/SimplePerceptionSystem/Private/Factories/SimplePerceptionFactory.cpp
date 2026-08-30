// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/SimplePerceptionFactory.h"

USenseBase* USimplePerceptionFactory::CreateSense(const USenseBase* SenseTemplate, USimplePerceiver* Perceiver)
{
	if (!IsValid(Perceiver) || !IsValid(SenseTemplate))
		return nullptr;

	USenseBase* NewSense = DuplicateObject<USenseBase>(SenseTemplate, Perceiver);
	if (NewSense)
		NewSense->Init(Perceiver);

	return NewSense;
}
