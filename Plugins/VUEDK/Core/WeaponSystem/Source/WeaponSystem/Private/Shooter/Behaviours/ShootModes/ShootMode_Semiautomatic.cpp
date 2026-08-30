// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShootModes/ShootMode_Semiautomatic.h"

bool UShootMode_Semiautomatic::OnRequestShoot_Implementation(EShootType ShootType)
{
	FinishShootRequest(true, true, false);
	return Super::OnRequestShoot_Implementation(ShootType);
}

void UShootMode_Semiautomatic::OnEndSequence_Implementation()
{
	CompleteProcessingRequest();
}
