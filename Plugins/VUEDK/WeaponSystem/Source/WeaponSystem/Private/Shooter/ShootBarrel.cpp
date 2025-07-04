// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/ShootBarrel.h"
#include "Shooter/ShootPoint.h"

UShootBarrel::UShootBarrel()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<UShootPoint*> UShootBarrel::GetShootPointsChildren() const
{
	TArray<UShootPoint*> ShootPoints;
	for (UActorComponent* Component : GetAttachChildren())
	{
		if (UShootPoint* ShootPoint = Cast<UShootPoint>(Component); IsValid(ShootPoint))
			ShootPoints.Add(ShootPoint);
	}

	return ShootPoints;
}
