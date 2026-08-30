// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/Handlers/Base/ShooterHandlerBase.h"
#include "WeaponSystem.h"
#include "Shooter/Shooter.h"

void UShooterHandlerBase::Init(UShooterBehaviourBase* InBehaviour)
{
	Behaviour = InBehaviour;

	if (!IsValid(Behaviour))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterHandlerBase::Init: ShooterBehaviour in %s is null."), *GetName());
		return;
	}
	
	BindEvents();
	OnInit();
}

void UShooterHandlerBase::TickHandler(float DeltaTime)
{
}

void UShooterHandlerBase::OnInit()
{
}

void UShooterHandlerBase::BindEvents()
{
	Behaviour->OnBehaviourEnabled.AddDynamic(this, &UShooterHandlerBase::OnBehaviourEnabled);
	Behaviour->OnBehaviourDisabled.AddDynamic(this, &UShooterHandlerBase::OnBehaviourDisabled);
	Behaviour->OnBehaviourShootSuccess.AddDynamic(this, &UShooterHandlerBase::OnBehaviourShootSuccess);
	Behaviour->OnBehaviourShootFail.AddDynamic(this, &UShooterHandlerBase::OnBehaviourShootFail);
}

void UShooterHandlerBase::OnBehaviourEnabled(UShooterBehaviourBase* InBehaviour)
{
}

void UShooterHandlerBase::OnBehaviourDisabled(UShooterBehaviourBase* InBehaviour)
{
}

void UShooterHandlerBase::OnBehaviourShootSuccess(UShooterBehaviourBase* InBehaviour, UShootBarrel* ShootBarrel, int32 ShotIndex)
{
}

void UShooterHandlerBase::OnBehaviourShootFail(UShooterBehaviourBase* InBehaviour, EShootFailReason FailReason)
{
}

UShootData* UShooterHandlerBase::GetShootData() const
{
	if (!Check())
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterHandlerBase::GetShootData: Could not get ShootData from %s, Check failed."), *GetName());
		return nullptr;
	}

	return Behaviour->ShootData;
}

bool UShooterHandlerBase::Check() const
{
	return IsValid(Behaviour) && IsValid(Behaviour->ShootData);
}
