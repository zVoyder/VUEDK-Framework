// Copyright VUEDK, Inc. All Rights Reserved.

#include "Weapons/WeaponFirearm.h"
#include "WeaponSystem.h"

AWeaponFirearm::AWeaponFirearm()
{
	FirearmMontagesManager = CreateDefaultSubobject<UFirearmMontagesManager>(TEXT("FirearmMontagesManager"));
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("Shooter"));
	ShootBarrel = CreateDefaultSubobject<UShootBarrel>(TEXT("ShootBarrel"));
	ShootBarrel->SetupAttachment(WeaponMesh);
}

#if WITH_EDITOR
void AWeaponFirearm::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	AttachBarrelToSocket();
}
#endif

void AWeaponFirearm::Init(APawn* InOwner, UObject* InPayload)
{
	Super::Init(InOwner, InPayload);
	Shooter->SetOwner(InOwner);
	BindEvents();
}

void AWeaponFirearm::AddWeaponDynamicSpread(const float AddSpread, const float ChangeRate, const float RecoveryRate) const
{
	Shooter->ShooterBehaviour->AddDynamicSpread(AddSpread, ChangeRate, RecoveryRate);
}

void AWeaponFirearm::ResetCooldown() const
{
	Shooter->ShooterBehaviour->ResetCooldown();
}

void AWeaponFirearm::ResetSpread() const
{
	Shooter->ShooterBehaviour->ResetSpread(WeaponFirearmData.SpreadChangeRate);
}

void AWeaponFirearm::SetWeaponDamage(const float NewDamage)
{
	Super::SetWeaponDamage(NewDamage);
	Shooter->ShooterBehaviour->SetDamage(NewDamage);
}

void AWeaponFirearm::SetWeaponFireRate(const float NewFireRate) const
{
	Shooter->ShooterBehaviour->SetFireRate(NewFireRate);
}

void AWeaponFirearm::SetWeaponMagSize(const int32 NewMagSize) const
{
	Shooter->ShooterBehaviour->SetMagSize(NewMagSize);
}

void AWeaponFirearm::SetWeaponMaxRange(const float NewRange) const
{
	Shooter->ShooterBehaviour->SetMaxRange(NewRange);
}

void AWeaponFirearm::SetWeaponRecoilStrength(const float NewRecoilStrength) const
{
	Shooter->ShooterBehaviour->SetRecoilStrength(NewRecoilStrength);
}

void AWeaponFirearm::SetWeaponReloadTime(const float NewReloadTime)
{
	WeaponFirearmData.ReloadTime = NewReloadTime;
}

void AWeaponFirearm::SetCurrentAmmo(const int32 NewAmmo) const
{
	Shooter->ShooterBehaviour->SetCurrentAmmo(NewAmmo);
}

void AWeaponFirearm::SetWeaponAmmoType(UAmmoTypeData* NewAmmoType)
{
	if (!IsValid(NewAmmoType))
		return;

	WeaponFirearmData.AmmoType = NewAmmoType;
}

void AWeaponFirearm::SetWeaponShootType(const EShootType NewShootType) const
{
	Shooter->ShooterBehaviour->SetShootType(NewShootType);
}

void AWeaponFirearm::InstantSetSpread(const float InSpread, const bool bOverrideDefault) const
{
	Shooter->ShooterBehaviour->InstantSetSpread(InSpread, bOverrideDefault);
}

void AWeaponFirearm::SetWeaponSpread(const float InSpread, const bool bOverrideDefault) const
{
	Shooter->ShooterBehaviour->SetSpread(InSpread, WeaponFirearmData.SpreadChangeRate, bOverrideDefault);
}

FWeaponFirearmData AWeaponFirearm::GetWeaponFirearmData() const
{
	return WeaponFirearmData;
}

float AWeaponFirearm::GetWeaponFireRate() const
{
	return Shooter->ShooterBehaviour->GetFireRate();
}

float AWeaponFirearm::GetWeaponMagSize() const
{
	return Shooter->ShooterBehaviour->GetMagSize();
}

float AWeaponFirearm::GetWeaponMaxRange() const
{
	return Shooter->ShooterBehaviour->GetMaxRange();
}

float AWeaponFirearm::GetWeaponRecoilStrength() const
{
	return Shooter->ShooterBehaviour->GetRecoilStrength();
}

float AWeaponFirearm::GetWeaponReloadTime() const
{
	return WeaponFirearmData.ReloadTime;
}

int32 AWeaponFirearm::GetCurrentAmmo() const
{
	return Shooter->ShooterBehaviour->GetCurrentAmmo();
}

int32 AWeaponFirearm::GetNeededAmmo() const
{
	return GetWeaponMagSize() - GetCurrentAmmo();
}

UAmmoTypeData* AWeaponFirearm::GetWeaponAmmoType() const
{
	return WeaponFirearmData.AmmoType;
}

EShootType AWeaponFirearm::GetWeaponShootType() const
{
	return Shooter->ShooterBehaviour->GetShootType();
}

float AWeaponFirearm::GetWeaponMaxSpread() const
{
	return Shooter->GetShootData().MaxSpread;
}

float AWeaponFirearm::GetDefaultSpread() const
{
	return Shooter->GetShootData().DefaultSpread;
}

float AWeaponFirearm::GetWeaponSpread() const
{
	return Shooter->ShooterBehaviour->GetSpread();
}

void AWeaponFirearm::ResetToDefaultShootType() const
{
	SetWeaponShootType(DefaultShootType);
}

void AWeaponFirearm::ReloadWithMontage(UAmmoTypeData* AmmoData, const int32 Ammo)
{
	if (IsReloading())
		return;

	if (!IsSameAmmoType(AmmoData))
	{
		UE_LOG(LogWeaponSystem, Display, TEXT("AWeaponFirearm::StartReload: Ammo type is not compatible."));
		return;
	}

	if (!CanReload())
		return;

	bHasReloadInsertedAmmo = false;
	float WeaponPlayRate = 0.f;
	float CharacterPlayRate = 0.f;
	const FReloadEventData ReloadEventData = FReloadEventData(Ammo);
	SetReloadPayload(ReloadEventData);

	if (IsValid(FirearmMontagesManager->ReloadMontage.GetWeaponMontage()))
		WeaponPlayRate = FirearmMontagesManager->ReloadMontage.GetWeaponMontage()->GetPlayLength() / GetWeaponReloadTime();

	if (IsValid(FirearmMontagesManager->ReloadMontage.GetCharacterMontage()))
		CharacterPlayRate = FirearmMontagesManager->ReloadMontage.GetCharacterMontage()->GetPlayLength() / GetWeaponReloadTime();

	OnReloadStarted.Broadcast(ReloadEventData);
	StartWeaponMontage(
		FirearmMontagesManager->ReloadMontage,
		WeaponPlayRate,
		CharacterPlayRate
	);
}

void AWeaponFirearm::InterruptReload(const float CharacterBlendOutTime, const float WeaponBlendOutTime)
{
	StopWeaponMontageWithBlends(FirearmMontagesManager->ReloadMontage, WeaponBlendOutTime, CharacterBlendOutTime);
}

int32 AWeaponFirearm::Refill(const int32 Ammo) const
{
	return Shooter->ShooterBehaviour->Refill(Ammo);
}

void AWeaponFirearm::RefillAllMagazine() const
{
	Shooter->ShooterBehaviour->RefillAllMagazine();
}

void AWeaponFirearm::SetAim(const bool bIsEnabled)
{
	if (bIsEnabled)
		EnableAim();
	else
		DisableAim();

	bIsAimingDownSight = bIsEnabled;
}

bool AWeaponFirearm::IsAiming() const
{
	return bIsAimingDownSight;
}

bool AWeaponFirearm::HasJustShot() const
{
	return Shooter->ShooterBehaviour->HasJustShot();
}

bool AWeaponFirearm::IsReloading() const
{
	return IsPlayingWeaponMontage(FirearmMontagesManager->ReloadMontage);
}

void AWeaponFirearm::BeginPlay()
{
	Shooter->Init(ShootBarrel);
	SetWeaponDamage(WeaponData.Damage);
	SetWeaponShootType(DefaultShootType);
	SetAimRecoilModifier();
	AttachBarrelToSocket();
	Super::BeginPlay();
}

void AWeaponFirearm::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindEvents();
	Super::EndPlay(EndPlayReason);
}

void AWeaponFirearm::EnableAim()
{
	SetAimSpreadModifier();
	SetWeaponSpread(AimSpread, false);
	SetWeaponRecoilStrength(AdsRecoilStrength);
	OnEnableAim();
	OnAimEnabled.Broadcast();
}

void AWeaponFirearm::DisableAim()
{
	ResetAimWeaponSpread();
	ResetAimWeaponRecoil();
	OnDisableAim();
	OnAimDisabled.Broadcast();
}

bool AWeaponFirearm::CanReload_Implementation() const
{
	return true;
}

void AWeaponFirearm::OnReloadSuccess_Implementation(float Remain, float ReloadedAmmo)
{
}

void AWeaponFirearm::OnReloadFail_Implementation()
{
}

void AWeaponFirearm::OnEnableAim_Implementation()
{
}

void AWeaponFirearm::OnDisableAim_Implementation()
{
}

void AWeaponFirearm::OnCurrentAmmoChanged_Implementation(int32 CurrentAmmo, int32 MagSize)
{
}

void AWeaponFirearm::OnEndShootSequence_Implementation()
{
}

void AWeaponFirearm::OnMagEmpty_Implementation()
{
}

FReloadEventData AWeaponFirearm::GetReloadPayload() const
{
	return ReloadPayload;
}

bool AWeaponFirearm::NativeDeployWeaponAttack()
{
	if (!Super::NativeDeployWeaponAttack())
		return false;

	if (!IsValid(FirearmMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponFirearm::NativeDeployWeaponAttack: FirearmMontagesManager is not valid."));
		return false;
	}
	
	if (FirearmMontagesManager->IsBusy())
		return false;
	
	if (bCanDeployAttackIfReloading)
		return Shooter->Shoot();
	
	if (IsReloading())
		return false;
	
	return Shooter->Shoot();
}

void AWeaponFirearm::OnEndWeaponAttack_Implementation()
{
	Shooter->ShooterBehaviour->EndShootSequence();
}

void AWeaponFirearm::BindEvents()
{
	FirearmMontagesManager->ReloadMontage.OnMontageFinished.AddDynamic(this, &AWeaponFirearm::OnReloadMontageEnded);

	if (!IsValid(Shooter->ShooterBehaviour))
		return;
	
	Shooter->ShooterBehaviour->OnCurrentAmmoChanged.AddDynamic(this, &AWeaponFirearm::OnCurrentAmmoChanged);
	Shooter->ShooterBehaviour->OnEndShootSequence.AddDynamic(this, &AWeaponFirearm::OnEndShootSequence);
	Shooter->ShooterBehaviour->OnMagEmpty.AddDynamic(this, &AWeaponFirearm::OnMagEmpty);
}

void AWeaponFirearm::UnbindEvents()
{
	FirearmMontagesManager->ReloadMontage.OnMontageFinished.RemoveDynamic(this, &AWeaponFirearm::OnReloadMontageEnded);

	if (!IsValid(Shooter->ShooterBehaviour))
		return;
	
	Shooter->ShooterBehaviour->OnCurrentAmmoChanged.RemoveDynamic(this, &AWeaponFirearm::OnCurrentAmmoChanged);
	Shooter->ShooterBehaviour->OnEndShootSequence.RemoveDynamic(this, &AWeaponFirearm::OnEndShootSequence);
	Shooter->ShooterBehaviour->OnMagEmpty.RemoveDynamic(this, &AWeaponFirearm::OnMagEmpty);
}

void AWeaponFirearm::SetReloadPayload(const FReloadEventData InReloadPayload)
{
	ReloadPayload = InReloadPayload;
}

void AWeaponFirearm::ReloadInsertAmmo()
{
	if (!IsReloading())
		return;
	
	const FReloadEventData EventData = GetReloadPayload();
	bHasReloadInsertedAmmo = true;
	const float Remain = Shooter->ShooterBehaviour->Refill(EventData.AmmoToReload);
	OnReloadSuccess(Remain, EventData.AmmoToReload - Remain);
	OnReloadInsertedAmmo.Broadcast(EventData);
}

void AWeaponFirearm::OnReloadMontageEnded(const bool bInterrupted)
{
	const FReloadEventData EventData = GetReloadPayload();
	OnReloadEnded.Broadcast();
	
	if (bInterrupted && !bHasReloadInsertedAmmo)
	{
		OnReloadInterrupted.Broadcast(EventData);
		OnReloadFail();
		return;
	}

	if (!bHasReloadInsertedAmmo)
		ReloadInsertAmmo();
}

void AWeaponFirearm::SetAimSpreadModifier()
{
	const float Spread = Shooter->GetShootData().DefaultSpread;
	AimSpread = Spread - ((Spread / 100.f) * GetWeaponFirearmData().AimPrecisionIncrease);
}

void AWeaponFirearm::SetAimRecoilModifier()
{
	DefaultRecoilStrength = Shooter->ShooterBehaviour->GetRecoilStrength();
	AdsRecoilStrength = DefaultRecoilStrength - ((DefaultRecoilStrength / 100.f) * GetWeaponFirearmData().AimRecoilControlIncrease);
}

void AWeaponFirearm::AttachBarrelToSocket() const
{
	if (!IsValid(WeaponMesh) || !IsValid(ShootBarrel))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("WeaponMesh or ShootBarrel is not valid"));
		return;
	}

	if (WeaponMesh->DoesSocketExist(ShootBarrelSocketName))
		ShootBarrel->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShootBarrelSocketName);
	else
		ShootBarrel->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "None");
}

bool AWeaponFirearm::IsSameAmmoType(const UAmmoTypeData* AmmoData) const
{
	if (!IsValid(AmmoData))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponFirearm::IsSameAmmoType: AmmoData is not valid."));
		return false;
	}

	return AmmoData->AmmoID == GetWeaponFirearmData().AmmoType->AmmoID;
}

void AWeaponFirearm::ResetAimWeaponRecoil() const
{
	SetWeaponRecoilStrength(DefaultRecoilStrength);
}

void AWeaponFirearm::ResetAimWeaponSpread() const
{
	Shooter->ShooterBehaviour->ResetSpread(WeaponFirearmData.SpreadChangeRate);
}
