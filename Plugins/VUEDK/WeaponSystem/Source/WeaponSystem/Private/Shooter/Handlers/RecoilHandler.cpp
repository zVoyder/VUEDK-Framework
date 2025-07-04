// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Handlers/RecoilHandler.h"
#include "Shooter/Shooter.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"

void URecoilHandler::Tick(float DeltaTime)
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
	
	if (!ShootData->bHasRecoil || !IsValid(ShootData->RecoilCurve))
		return;

	const float RecoilPitch = ShootData->RecoilCurve->GetVectorValue(Behaviour->GetShotsFired()).Y;
	const float RecoilYaw = ShootData->RecoilCurve->GetVectorValue(Behaviour->GetShotsFired()).Z;
	ImpulseRecoil = FRotator(RecoilPitch, RecoilYaw, 0.0f);
	RecoilRemaining = ShootData->RecoilDuration;
}

void URecoilHandler::ProcessRecoilImpulseRotation(const float DeltaTime)
{
	if (RecoilRemaining <= 0.0f)
		return;
	
	const float NormalizedTime = FMath::Clamp(RecoilRemaining / ShootData->RecoilDuration, -1.f, 1.f);
	const float DecayFactor = ShootData->RecoilCurve->GetVectorValue(1.0f - NormalizedTime).X;
	const FRotator RecoilStep = ImpulseRecoil * (DecayFactor * DeltaTime * Behaviour->GetRecoilStrength() * RecoilStrengthMultiplier);
	RecoilRemaining -= DeltaTime;

	APawn* Owner = Behaviour->GetOwner();
	Owner->AddControllerPitchInput(-RecoilStep.Pitch);
	Owner->AddControllerYawInput(RecoilStep.Yaw);
}

bool URecoilHandler::Check() const
{
	return Super::Check() && IsValid(Behaviour->GetOwner());
}
