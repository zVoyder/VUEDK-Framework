// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/ShootPoint.h"
#include "WeaponSystem.h"
#include "Shooter/ShootBarrel.h"
#include "Shooter/Shooter.h"

UShootPoint::UShootPoint()
{
	PrimaryComponentTick.bCanEverTick = false;

#if WITH_EDITOR
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(this);
	ArrowComponent->ArrowSize = 0.2f;
	ArrowComponent->SetArrowColor(FColor::White);
#endif
}

void UShootPoint::SetSpreadDegree(const float NewSpreadDegree)
{
	SpreadRad = FMath::DegreesToRadians(NewSpreadDegree);
}

void UShootPoint::GenerateSpreadDegree(const float MaxSpreadDegree)
{
	SetSpreadDegree(FMath::RandRange(0.f, MaxSpreadDegree));
}

float UShootPoint::GetSpreadRad() const
{
	return SpreadRad;
}

FVector UShootPoint::GetShootPointLocation() const
{
	return GetComponentLocation();
}

FVector UShootPoint::GetShootPointRelativeLocation() const
{
	return GetRelativeLocation();
}

FVector UShootPoint::GetShootPointDirection() const
{
	return GetForwardVector();
}

FVector UShootPoint::GetShootPointRelativeDirection() const
{
	return GetRelativeRotation().Vector();
}

FVector UShootPoint::GetShootPointSpreadedDirection() const
{
	return GetShootPointDirection() + FMath::VRandCone(GetShootPointDirection(), SpreadRad);
}

FVector UShootPoint::GetShootPointSpreadedRelativeDirection() const
{
	return GetShootPointRelativeDirection() + FMath::VRandCone(GetShootPointRelativeDirection(), SpreadRad);
}

void UShootPoint::OnRegister()
{
	Super::OnRegister();

	if (!GetAttachParent()->IsA(UShootBarrel::StaticClass()))
		UE_LOG(LogShooter, Error, TEXT("UShootPoint::OnRegister: ShootPoint %s in %s is not in a ShootBarrel."), *GetName(), *GetAttachParentActor()->GetName());
}
