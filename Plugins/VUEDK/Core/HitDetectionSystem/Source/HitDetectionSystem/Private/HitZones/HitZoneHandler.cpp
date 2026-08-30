// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitZones/HitZoneHandler.h"
#include "HitDetectionSystem.h"

void UHitZoneHandler::Init(AActor* InOwner, UPrimitiveComponent* InHitBox)
{
	Owner = InOwner;
	HitBox = InHitBox;

	if (!Check())
	{
		UE_LOG(LogHitDetectionSystem, Warning, TEXT("UHitBoxHandler::Init: Owner is not valid."));
		return;
	}

	Owner->OnTakePointDamage.AddUniqueDynamic(this, &UHitZoneHandler::OnTakePointDamage);
	Owner->OnTakeRadialDamage.AddUniqueDynamic(this, &UHitZoneHandler::OnTakeRadialDamage);
}

bool UHitZoneHandler::Check() const
{
	return IsValid(Owner) && IsValid(HitBox);
}

void UHitZoneHandler::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (FHitComponent != HitBox)
		return;
	
	const float TotalDamage = ProcessPointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	OnZoneHitPointDamage.Broadcast(Damage, TotalDamage, HitLocation, ShotFromDirection, GetBoneName(), DamageType, InstigatedBy, DamageCauser);
}

void UHitZoneHandler::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bReceiveRadialDamage)
		return;

	const float TotalDamage = ProcessRadialDamage(DamagedActor, Damage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
	OnZoneHitRadialDamage.Broadcast(Damage, TotalDamage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
}

FName UHitZoneHandler::GetBoneName() const
{
	switch (BoneRedirectorMode)
	{
	case EHitZoneRedirectorMode::None:
		return NAME_None;
	case EHitZoneRedirectorMode::CustomName:
		return BoneRedirector;
	case EHitZoneRedirectorMode::AttachedBoneName:
		{
			if (!IsValid(HitBox))
				return NAME_None;

			return HitBox->GetAttachSocketName();
		}
	}

	return NAME_None;
}

float UHitZoneHandler::ProcessPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	for (UDamageProcessor* Processor : DamageProcessors)
	{
		if (!IsValid(Processor))
			continue;
		
		Damage = Processor->ProcessAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
		Damage = Processor->ProcessPointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	}

	return Damage * Multiplier;
}

float UHitZoneHandler::ProcessRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	for (UDamageProcessor* Processor : DamageProcessors)
	{
		if (!IsValid(Processor))
			continue;

		Damage = Processor->ProcessAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
		Damage = Processor->ProcessRadialDamage(DamagedActor, Damage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
	}

	return Damage * Multiplier;
}
