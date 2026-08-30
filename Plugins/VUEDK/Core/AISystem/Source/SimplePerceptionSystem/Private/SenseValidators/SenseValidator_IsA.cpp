// Copyright VUEDK, Inc. All Rights Reserved.

#include "SenseValidators/SenseValidator_IsA.h"

bool USenseValidator_IsA::ReceiveValidate_Implementation(AActor* Actor) const
{
	return Actor->GetClass()->IsChildOf(ActorClass);
}
