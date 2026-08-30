// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShootModes/ShootMode_Automatic.h"

bool UShootMode_Automatic::OnRequestShoot_Implementation(EShootType ShootType)
{
	FinishShootRequest();
	return Super::OnRequestShoot_Implementation(ShootType);
}
