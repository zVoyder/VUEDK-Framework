// Copyright VUEDK, Inc. All Rights Reserved.

#include "Weapons/WeaponBase.h"
#include "WeaponSystem.h"
#include "Factories/WeaponAnimFactory.h"
#include "GameFramework/Character.h"
#include "Montages/Data/WeaponAnimMetaData.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponRoot);
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponRoot);
}

void AWeaponBase::Init(APawn* InOwner, UObject* InPayload)
{
	SetPayload(InPayload);
	SetOwner(InOwner);
	SetInstigator(InOwner);
	SetWeaponDamage(GetWeaponDamage());

	UWeaponMontagesManagerBase* MontagesManager = GetMontagesManager();
	if (!IsValid(MontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponBase::Init: MontagesManager is not valid for weapon %s."), *GetName());
		return;
	}
	
	MontagesManager->Init(this);
}

USkeletalMeshComponent* AWeaponBase::GetWeaponMesh() const
{
	return WeaponMesh;
}

void AWeaponBase::Equip()
{
	if (bIsEquipped)
		return;

	bIsEquipped = true;
	OnWeaponEquipped();
	OnWeaponEquippedEvent.Broadcast();
}

void AWeaponBase::Unequip()
{
	if (!bIsEquipped)
		return;

	bIsEquipped = false;
	OnWeaponUnequipped();
	OnWeaponUnequippedEvent.Broadcast();
}

bool AWeaponBase::WeaponAttack()
{
	if (NativeDeployWeaponAttack())
	{
		bIsWeaponAttacking = true;
		OnWeaponAttackSuccess();
		OnWeaponAttacked.Broadcast();
		return true;
	}

	bIsWeaponAttacking = false;
	OnWeaponAttackFail();
	OnWeaponAttackFailed.Broadcast();
	return false;
}

void AWeaponBase::EndWeaponAttack()
{
	bIsWeaponAttacking = false;
	OnEndWeaponAttack();
	OnWeaponAttackEnded.Broadcast();
}

void AWeaponBase::SetPayload(UObject* InPayload)
{
	Payload = InPayload;
}

void AWeaponBase::SetWeaponDamage(const float NewDamage)
{
	WeaponData.Damage = NewDamage;
}

float AWeaponBase::GetWeaponDamage() const
{
	return WeaponData.Damage;
}

UObject* AWeaponBase::GetPayload() const
{
	return Payload;
}

bool AWeaponBase::IsEquipped() const
{
	return bIsEquipped;
}

bool AWeaponBase::IsWeaponAttacking() const
{
	return bIsWeaponAttacking;
}

FWeaponData AWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponData.bAutoInitialize && IsValid(Owner))
	{
		if (APawn* OwnerPawn = Cast<APawn>(Owner))
			Init(OwnerPawn);
	}
}

UWeaponMontagesManagerBase* AWeaponBase::GetMontagesManager() const
{
	return nullptr;
}

bool AWeaponBase::NativeDeployWeaponAttack()
{
	return DeployWeaponAttack();
}

void AWeaponBase::OnEndWeaponAttack_Implementation()
{
}

bool AWeaponBase::DeployWeaponAttack_Implementation()
{
	return true;
}

void AWeaponBase::OnWeaponAttackSuccess_Implementation()
{
}

void AWeaponBase::OnWeaponAttackFail_Implementation()
{
}

void AWeaponBase::OnWeaponEquipped_Implementation()
{
}

void AWeaponBase::OnWeaponUnequipped_Implementation()
{
}
