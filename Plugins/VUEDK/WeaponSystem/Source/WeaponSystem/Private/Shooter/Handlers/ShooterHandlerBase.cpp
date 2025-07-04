// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Handlers/ShooterHandlerBase.h"
#include "Shooter/Shooter.h"

void UShooterHandlerBase::Init(UShooterBehaviourBase* InBehaviour, FShootData& InShootData)
{
	Behaviour = InBehaviour;
	ShootData = &InShootData;

	if (!IsValid(Behaviour))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterHandlerBase::Init: ShooterBehaviour in %s is null."), *GetName());
		return;
	}

	if (ShootData == nullptr)
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterHandlerBase::Init: ShootData in %s is null."), *GetName());
		return;
	}
	
	bIsInitialized = true;
	BindEvents();
	OnInit();
}

void UShooterHandlerBase::Tick(float DeltaTime)
{
}

bool UShooterHandlerBase::IsTickable() const
{
	return Check();
}

TStatId UShooterHandlerBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UShooterHandlerBase, STATGROUP_Tickables);
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

void UShooterHandlerBase::OnBehaviourEnabled()
{
}

void UShooterHandlerBase::OnBehaviourDisabled()
{
}

void UShooterHandlerBase::OnBehaviourShootSuccess(UShootBarrel* ShootBarrel, int32 ShotIndex)
{
}

void UShooterHandlerBase::OnBehaviourShootFail()
{
}

bool UShooterHandlerBase::Check() const
{
	return bIsInitialized;
}
