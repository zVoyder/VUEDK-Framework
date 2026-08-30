// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/Handlers/RecoilHandler.h"
#include "WeaponSystem.h"
#include "Shooter/Shooter.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"

void URecoilHandler::TickHandler(float DeltaTime)
{
	ProcessRecoilImpulseRotation(DeltaTime);
}

void URecoilHandler::ApplyRecoilImpulse()
{
	if (!Check())
	{
		UE_LOG(LogShooter, Warning, TEXT("URecoilHandler::ApplyRecoilImpulse: Could not apply recoil to %s, Check failed."), *GetName());
		return;
	}

	const UShootData* ShootData = GetShootData();
	if (!IsValid(ShootData))
		return;
	
	if (!ShootData->bHasRecoil || !IsValid(ShootData->RecoilCurve))
		return;

	const float RecoilPitch = ShootData->RecoilCurve->GetVectorValue(Behaviour->GetShootHandledRequestsCount()).Y;
	const float RecoilYaw = ShootData->RecoilCurve->GetVectorValue(Behaviour->GetShootHandledRequestsCount()).Z;
	ImpulseRecoil = FRotator(RecoilPitch, RecoilYaw, 0.0f);
	RecoilRemaining = ShootData->RecoilDuration;
}

void URecoilHandler::ProcessRecoilImpulseRotation(const float DeltaTime)
{
	if (RecoilRemaining <= 0.0f)
		return;

	if (!Check())
	{
		UE_LOG(LogShooter, Warning, TEXT("URecoilHandler::ApplyRecoilImpulse: Could not apply recoil to %s, Check failed."), *GetName());
		return;
	}
	
	const UShootData* ShootData = GetShootData();
	const float NormalizedTime = FMath::Clamp(RecoilRemaining / ShootData->RecoilDuration, -1.f, 1.f);
	const float DecayFactor = ShootData->RecoilCurve->GetVectorValue(1.0f - NormalizedTime).X;
	const FRotator RecoilStep = ImpulseRecoil * (DecayFactor * DeltaTime * Behaviour->GetRecoilStrength() * RecoilStrengthMultiplier);
	RecoilRemaining -= DeltaTime;

	APawn* Owner = Behaviour->GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogShooter, Warning, TEXT("URecoilHandler::ApplyRecoilImpulse: Could not apply recoil to %s, Owner is invalid."), *GetName());
		return;
	}
	
	Owner->AddControllerPitchInput(-RecoilStep.Pitch);
	Owner->AddControllerYawInput(RecoilStep.Yaw);
}
