// Copyright VUEDK, Inc. All Rights Reserved.

#include "SenseValidators/Base/SenseValidatorBase.h"

bool USenseValidatorBase::Validate(AActor* Actor) const
{
	return ReceiveValidate(Actor);
}

bool USenseValidatorBase::ReceiveValidate_Implementation(AActor* Actor) const
{
	return false;
}
