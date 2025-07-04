// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitZones/HitZoneCapsule.h"

UHitZoneCapsule::UHitZoneCapsule(): HitZoneHandler(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	ShapeColor = FColor::Red;
}

UHitZoneHandler* UHitZoneCapsule::GetHitZoneHandler_Implementation() const
{
	return HitZoneHandler;
}

void UHitZoneCapsule::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(HitZoneHandler))
		return;

	HitZoneHandler->Init(GetOwner(), this);
}
