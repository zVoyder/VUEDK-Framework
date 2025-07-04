// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitZones/HitZoneHandler.h"
#include "HitDetectionSystem.h"

void UHitZoneHandler::Init(AActor* InOwner, UPrimitiveComponent* InHitBox)
{
	Owner = InOwner;
	HitBox = InHitBox;

	if (!Check())
	{
		UE_LOG(LogHitDetectionSystem, Warning, TEXT("UHitBoxHandler::Init: Owner is not valid!"));
		return;
	}

	Owner->OnTakePointDamage.AddDynamic(this, &UHitZoneHandler::OnTakePointDamage);
	Owner->OnTakeRadialDamage.AddDynamic(this, &UHitZoneHandler::OnTakeRadialDamage);
}

bool UHitZoneHandler::Check() const
{
	return IsValid(Owner) && IsValid(HitBox);
}

void UHitZoneHandler::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (FHitComponent != HitBox)
		return;

	const float TotalDamage = Damage * Multiplier;
	OnZoneHitAnyDamage.Broadcast(Damage, TotalDamage, DamageType, InstigatedBy, DamageCauser);
	OnZoneHitPointDamage.Broadcast(Damage, TotalDamage, HitLocation, ShotFromDirection, GetBoneName(), DamageType, InstigatedBy, DamageCauser);
}

void UHitZoneHandler::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bReceiveRadialDamage)
		return;
	
	const float TotalDamage = Damage * Multiplier;
	OnZoneHitAnyDamage.Broadcast(Damage, TotalDamage, DamageType, InstigatedBy, DamageCauser);
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
