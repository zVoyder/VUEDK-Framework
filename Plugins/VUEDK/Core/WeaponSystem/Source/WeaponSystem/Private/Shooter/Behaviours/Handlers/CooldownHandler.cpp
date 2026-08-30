// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/Handlers/CooldownHandler.h"
#include "WeaponSystem.h"
#include "Shooter/Shooter.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"

void UCooldownHandler::TickHandler(const float DeltaTime)
{
	ProcessCooldown(DeltaTime);
}

void UCooldownHandler::ResetCooldown()
{
	CooldownRemaining = 0.0f;
	bIsInCooldown = false;
}

bool UCooldownHandler::IsInCooldown() const
{
	return bIsInCooldown;
}

void UCooldownHandler::StartCooldown()
{
	const float FireRate = Behaviour->GetFireRate();

	if (FireRate <= 0.0f)
	{
		UE_LOG(LogShooter, Warning, TEXT("UCooldownHandler::StartCooldown: FireRate is less than or equal to zero. Setting cooldown to zero."));
		ResetCooldown();
		return;
	}
	
	const float Cooldown = 1.0f / (Behaviour->GetFireRate() / 60.0f);
	CooldownRemaining = Cooldown;
	bIsInCooldown = true;
}

void UCooldownHandler::ProcessCooldown(const float DeltaTime)
{
	if (!bIsInCooldown)
		return;
	
	CooldownRemaining -= DeltaTime;
	if (CooldownRemaining <= 0.0f)
		EndShootCooldown();
}

void UCooldownHandler::EndShootCooldown()
{
	ResetCooldown();
}
