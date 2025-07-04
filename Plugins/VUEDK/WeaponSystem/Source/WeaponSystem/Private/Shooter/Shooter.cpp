// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Shooter.h"

UShooter::UShooter(): ShooterBehaviour(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShooter::Init(UShootBarrel* InShootBarrel)
{
	if (!Check())
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::Init: Check Failed."));
		return;
	}
	
	ShooterBehaviour->Init(this, ShootData, InShootBarrel);
}

void UShooter::SetOwner(APawn* InOwner) const
{
	ShooterBehaviour->SetOwner(InOwner);
}

bool UShooter::Shoot() const
{
	if (!Check())
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::Shoot: Check failed."));
		return false;
	}
	
	return ShooterBehaviour->Shoot();
}

FShootData UShooter::GetShootData() const
{
	return ShootData;
}

void UShooter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check())
		return;
	
	ShooterBehaviour->DisableBehaviour();
}

void UShooter::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!Check())
		return;

	ShooterBehaviour->TickBehaviour(DeltaTime);
}

bool UShooter::Check() const
{
	return IsValid(ShooterBehaviour);
}
