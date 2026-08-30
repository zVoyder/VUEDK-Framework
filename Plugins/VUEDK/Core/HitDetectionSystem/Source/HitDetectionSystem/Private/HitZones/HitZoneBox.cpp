// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitZones/HitZoneBox.h"

UHitZoneBox::UHitZoneBox(): HitZoneHandler(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	ShapeColor = FColor::Red;
}

UHitZoneHandler* UHitZoneBox::GetHitZoneHandler_Implementation() const
{
	return HitZoneHandler;
}

void UHitZoneBox::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(HitZoneHandler))
		return;

	HitZoneHandler->Init(GetOwner(), this);
}
