// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitZones/HitZoneSphere.h"

UHitZoneSphere::UHitZoneSphere(): HitZoneHandler(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	ShapeColor = FColor::Red;
}

UHitZoneHandler* UHitZoneSphere::GetHitZoneHandler_Implementation() const
{
	return HitZoneHandler;
}

void UHitZoneSphere::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(HitZoneHandler))
		return;

	HitZoneHandler->Init(GetOwner(), this);
}
