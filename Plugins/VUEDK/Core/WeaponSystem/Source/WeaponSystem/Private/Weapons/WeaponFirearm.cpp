// Copyright VUEDK, Inc. All Rights Reserved.

#include "Weapons/WeaponFirearm.h"
#include "WeaponSystem.h"
#include "Weapons/Data/WeaponShootData.h"
#include "Weapons/Managers/ReloadManager.h"

AWeaponFirearm::AWeaponFirearm()
{
	FirearmMontagesManager = CreateDefaultSubobject<UFirearmMontagesManager>(TEXT("FirearmMontagesManager"));
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("Shooter"));
	ReloadManager = CreateDefaultSubobject<UReloadManager>(TEXT("ReloadManager"));
	MainShootBarrel = CreateDefaultSubobject<UShootBarrel>(TEXT("ShootBarrel"));
	MainShootBarrel->SetupAttachment(WeaponMesh);
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
	Shooter->SetupShootBarrel(MainShootBarrel);
	Shooter->Init(InOwner);
	ReloadManager->Init(this);
	SetAimRecoilModifier();
	BindEvents();
}

UWeaponMontagesManagerBase* AWeaponFirearm::GetMontagesManager() const
{
	return FirearmMontagesManager;
}

bool AWeaponFirearm::Shoot(const int32 BehaviourIndex)
{
	if (!CanShoot())
		return false;
	
	return Shooter->Shoot(BehaviourIndex);
}

void AWeaponFirearm::EndShootSequence(const int32 BehaviourIndex) const
{
	Shooter->EndShootSequence(BehaviourIndex);
}

bool AWeaponFirearm::IsMagFull(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return false;

	return Behaviour->IsMagazineFull();
}

bool AWeaponFirearm::IsMagEmpty(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return false;

	return Behaviour->IsMagazineEmpty();
}

void AWeaponFirearm::AddWeaponDynamicSpread(const float AddSpread, const float ChangeRate, const float RecoveryRate, const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->AddDynamicSpread(AddSpread, ChangeRate, RecoveryRate);
}

void AWeaponFirearm::ResetCooldown(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->ResetCooldown();
}

void AWeaponFirearm::ResetSpread(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->ResetSpread(WeaponFirearmData.SpreadChangeRate);
}

void AWeaponFirearm::SetWeaponDamage(const float NewDamage)
{
	Super::SetWeaponDamage(NewDamage);
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(0);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetDamage(NewDamage);
}

void AWeaponFirearm::SetDamage(const float NewDamage, const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetDamage(NewDamage);
}

void AWeaponFirearm::SetWeaponFireRate(const float NewFireRate, const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetFireRate(NewFireRate);
}

void AWeaponFirearm::SetWeaponMagazineSize(const int32 NewSize, const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetMagSize(NewSize);
}

void AWeaponFirearm::SetWeaponMaxRange(const float NewRange, const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetMaxRange(NewRange);
}

void AWeaponFirearm::SetWeaponRecoilStrength(const float NewRecoilStrength, const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetRecoilStrength(NewRecoilStrength);
}

void AWeaponFirearm::SetWeaponReloadTime(const float NewReloadTime)
{
	WeaponFirearmData.ReloadTime = NewReloadTime;
}

void AWeaponFirearm::SetCurrentAmmo(const int32 NewAmmo, const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	UMagazine* RelatedMagazine = Behaviour->GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return;

	RelatedMagazine->SetCurrentAmmo(NewAmmo);
}

void AWeaponFirearm::SetWeaponShootType(const EShootType NewShootType, const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->ChangeShootType(NewShootType);
}

void AWeaponFirearm::InstantSetSpread(const float InSpread, const bool bOverrideDefault, const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->InstantSetSpread(InSpread, bOverrideDefault);
}

void AWeaponFirearm::SetWeaponSpread(const float InSpread, const bool bOverrideDefault, const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->SetSpread(InSpread, WeaponFirearmData.SpreadChangeRate, bOverrideDefault);
}

UShooterBehaviourBase* AWeaponFirearm::GetShooterBehaviour(const int32 BehaviourIndex) const
{
	return Shooter->GetShooterBehaviour(BehaviourIndex);
}

UMagazine* AWeaponFirearm::GetWeaponMagazine(const int32 MagazineIndex) const
{
	return Shooter->GetMagazine(MagazineIndex);
}

FWeaponFirearmData AWeaponFirearm::GetWeaponFirearmData() const
{
	return WeaponFirearmData;
}

UReloadManager* AWeaponFirearm::GetReloadManager() const
{
	return ReloadManager;
}

float AWeaponFirearm::GetWeaponFireRate(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetFireRate();
}

int32 AWeaponFirearm::GetWeaponMagSize(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetMagazineSize();
}

float AWeaponFirearm::GetWeaponMaxRange(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetMaxRange();
}

float AWeaponFirearm::GetWeaponRecoilStrength(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetRecoilStrength();
}

float AWeaponFirearm::GetWeaponReloadTime() const
{
	return WeaponFirearmData.ReloadTime;
}

int32 AWeaponFirearm::GetCurrentAmmo(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0;

	const UMagazine* RelatedMagazine = Behaviour->GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return 0;

	return RelatedMagazine->GetCurrentAmmo();
}

int32 AWeaponFirearm::GetNeededAmmo(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0;

	const UMagazine* RelatedMagazine = Behaviour->GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return 0;

	return RelatedMagazine->GetNeededAmmoToFull();
}

UAmmoTypeData* AWeaponFirearm::GetWeaponAmmoType(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return nullptr;

	const UMagazine* RelatedMagazine = Behaviour->GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return nullptr;

	return RelatedMagazine->GetAmmoType();
}

EShootType AWeaponFirearm::GetWeaponShootType(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return EShootType::None;

	return Behaviour->GetShootType();
}

float AWeaponFirearm::GetWeaponMaxSpread(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetMaxRange();
}

float AWeaponFirearm::GetDefaultSpread(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetDefaultSpread();
}

float AWeaponFirearm::GetWeaponSpread(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return 0.f;

	return Behaviour->GetSpread();
}

void AWeaponFirearm::ResetToDefaultShootType(const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->ResetShootType();
}

void AWeaponFirearm::Reload(const FReloadRequest& Request) const
{
	if (!CanReload())
		return;

	ReloadManager->RequestReload(Request);
}

void AWeaponFirearm::FullReload(const EReloadMode ReloadMode) const
{
	if (!CanReload())
		return;

	ReloadManager->RequestFullReload(ReloadMode);
}

void AWeaponFirearm::InterruptReload(const float CharacterBlendOutTime, const float WeaponBlendOutTime) const
{
	ReloadManager->InterruptReload();
}

void AWeaponFirearm::SetAim(const bool bIsEnabled, const int32 BehaviourIndex)
{
	if (bIsEnabled)
		EnableAim(BehaviourIndex);
	else
		DisableAim(BehaviourIndex);

	bIsAimingDownSight = bIsEnabled;
}

bool AWeaponFirearm::IsAiming() const
{
	return bIsAimingDownSight;
}

bool AWeaponFirearm::IsReloading() const
{
	if (!IsValid(ReloadManager))
		return false;

	return ReloadManager->IsReloading();
}

void AWeaponFirearm::BeginPlay()
{
	AttachBarrelToSocket();
	Super::BeginPlay();
}

void AWeaponFirearm::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindEvents();
	Super::EndPlay(EndPlayReason);
}

bool AWeaponFirearm::CanShoot()
{
	if (!IsValid(FirearmMontagesManager))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AWeaponFirearm::NativeDeployWeaponAttack: FirearmMontagesManager is not valid."));
		return false;
	}

	if (FirearmMontagesManager->IsBusy())
		return false;

	if (WeaponFirearmData.bCanDeployAttackIfReloading)
		return true;

	if (IsReloading())
		return false;

	return true;
}

bool AWeaponFirearm::NativeDeployWeaponAttack()
{
	if (!Super::NativeDeployWeaponAttack())
		return false;

	if (!CanShoot())
		return false;

	return Shooter->Shoot();
}

void AWeaponFirearm::OnEndWeaponAttack_Implementation()
{
	Shooter->EndShootSequence();
}

void AWeaponFirearm::EnableAim(const int32 BehaviourIndex)
{
	SetAimSpreadModifier(BehaviourIndex);
	SetWeaponSpread(AimSpread, false, BehaviourIndex);
	SetWeaponRecoilStrength(AdsRecoilStrength, BehaviourIndex);
	OnEnableAim();
	OnAimEnabled.Broadcast();
}

void AWeaponFirearm::DisableAim(const int32 BehaviourIndex)
{
	ResetAimWeaponSpread(BehaviourIndex);
	ResetAimWeaponRecoil(BehaviourIndex);
	OnDisableAim();
	OnAimDisabled.Broadcast();
}

void AWeaponFirearm::OnBehaviourEnable_Implementation(UShooterBehaviourBase* Behaviour)
{
}

bool AWeaponFirearm::CanReload_Implementation() const
{
	return true;
}

void AWeaponFirearm::OnEnableAim_Implementation()
{
}

void AWeaponFirearm::OnDisableAim_Implementation()
{
}

void AWeaponFirearm::OnBehaviourDisable_Implementation(UShooterBehaviourBase* Behaviour)
{
}

void AWeaponFirearm::OnBehaviourShootSuccess_Implementation(UShooterBehaviourBase* Behaviour, UShootBarrel* ShootBarrel, int32 ShotIndex)
{
}

void AWeaponFirearm::OnBehaviourShootFail_Implementation(UShooterBehaviourBase* Behaviour, EShootFailReason FailReason)
{
}

void AWeaponFirearm::OnBehaviourShootSequenceEnd_Implementation(UShooterBehaviourBase* Behaviour)
{
}

void AWeaponFirearm::OnBehaviourAmmoChange_Implementation(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize)
{
}

void AWeaponFirearm::OnBehaviourRefill_Implementation(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
}

void AWeaponFirearm::OnBehaviourFull_Implementation(UShooterBehaviourBase* Behaviour, UMagazine* Magazine)
{
}

void AWeaponFirearm::OnBehaviourEmpty_Implementation(UShooterBehaviourBase* Behaviour, UMagazine* Magazine)
{
}

void AWeaponFirearm::OnMagazineRefill_Implementation(const UObject* MagInstigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
}

void AWeaponFirearm::OnMagazineAmmoChange_Implementation(const UObject* MagInstigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize)
{
}

void AWeaponFirearm::OnMagazineFull_Implementation(const UObject* MagInstigator, const UMagazine* Magazine)
{
}

void AWeaponFirearm::OnMagazineEmpty_Implementation(const UObject* MagInstigator, const UMagazine* Magazine)
{
}

void AWeaponFirearm::OnReloadStart_Implementation(const FReloadRequest& Request)
{
}

void AWeaponFirearm::OnReloadEnd_Implementation(const FReloadRequest& Request, bool bInterrupted)
{
}

void AWeaponFirearm::OnReloadInsertAmmo_Implementation(UShooterBehaviourBase* Behaviour, int32 InsertedAmmo, int32 RemainingAmmo)
{
}

void AWeaponFirearm::BindEvents()
{
	Shooter->OnBehaviourEnabled.AddDynamic(this, &AWeaponFirearm::OnBehaviourEnable);
	Shooter->OnBehaviourDisabled.AddDynamic(this, &AWeaponFirearm::OnBehaviourDisable);
	Shooter->OnBehaviourShootSuccess.AddDynamic(this, &AWeaponFirearm::OnBehaviourShootSuccess);
	Shooter->OnBehaviourShootFail.AddDynamic(this, &AWeaponFirearm::OnBehaviourShootFail);
	Shooter->OnBehaviourShootSequenceEnded.AddDynamic(this, &AWeaponFirearm::OnBehaviourShootSequenceEnd);
	Shooter->OnBehaviourAmmoChanged.AddDynamic(this, &AWeaponFirearm::OnBehaviourAmmoChange);
	Shooter->OnBehaviourRefilled.AddDynamic(this, &AWeaponFirearm::OnBehaviourRefill);
	Shooter->OnBehaviourFull.AddDynamic(this, &AWeaponFirearm::OnBehaviourFull);
	Shooter->OnBehaviourEmpty.AddDynamic(this, &AWeaponFirearm::OnBehaviourEmpty);

	Shooter->OnMagazineRefilled.AddDynamic(this, &AWeaponFirearm::OnMagazineRefill);
	Shooter->OnMagazineAmmoChanged.AddDynamic(this, &AWeaponFirearm::OnMagazineAmmoChange);
	Shooter->OnMagazineFull.AddDynamic(this, &AWeaponFirearm::OnMagazineFull);
	Shooter->OnMagazineEmpty.AddDynamic(this, &AWeaponFirearm::OnMagazineEmpty);

	ReloadManager->OnReloadStarted.AddDynamic(this, &AWeaponFirearm::OnReloadStart);
	ReloadManager->OnReloadEnded.AddDynamic(this, &AWeaponFirearm::OnReloadEnd);
	ReloadManager->OnReloadInsertedAmmo.AddDynamic(this, &AWeaponFirearm::OnReloadInsertAmmo);
}

void AWeaponFirearm::UnbindEvents()
{
	Shooter->OnBehaviourEnabled.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourEnable);
	Shooter->OnBehaviourDisabled.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourDisable);
	Shooter->OnBehaviourShootSuccess.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourShootSuccess);
	Shooter->OnBehaviourShootFail.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourShootFail);
	Shooter->OnBehaviourShootSequenceEnded.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourShootSequenceEnd);
	Shooter->OnBehaviourAmmoChanged.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourAmmoChange);
	Shooter->OnBehaviourRefilled.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourRefill);
	Shooter->OnBehaviourFull.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourFull);
	Shooter->OnBehaviourEmpty.RemoveDynamic(this, &AWeaponFirearm::OnBehaviourEmpty);

	Shooter->OnMagazineRefilled.RemoveDynamic(this, &AWeaponFirearm::OnMagazineRefill);
	Shooter->OnMagazineAmmoChanged.RemoveDynamic(this, &AWeaponFirearm::OnMagazineAmmoChange);
	Shooter->OnMagazineFull.RemoveDynamic(this, &AWeaponFirearm::OnMagazineFull);
	Shooter->OnMagazineEmpty.RemoveDynamic(this, &AWeaponFirearm::OnMagazineEmpty);

	ReloadManager->OnReloadStarted.RemoveDynamic(this, &AWeaponFirearm::OnReloadStart);
	ReloadManager->OnReloadEnded.RemoveDynamic(this, &AWeaponFirearm::OnReloadEnd);
	ReloadManager->OnReloadInsertedAmmo.RemoveDynamic(this, &AWeaponFirearm::OnReloadInsertAmmo);
}

void AWeaponFirearm::SetAimSpreadModifier(const int32 BehaviourIndex)
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	const float Spread = Behaviour->GetSpread();
	AimSpread = Spread - ((Spread / 100.f) * GetWeaponFirearmData().AimPrecisionIncrease);
}

void AWeaponFirearm::SetAimRecoilModifier(const int32 BehaviourIndex)
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	DefaultRecoilStrength = Behaviour->GetRecoilStrength();
	AdsRecoilStrength = DefaultRecoilStrength - ((DefaultRecoilStrength / 100.f) * GetWeaponFirearmData().AimRecoilControlIncrease);
}

void AWeaponFirearm::ResetAimWeaponRecoil(const int32 BehaviourIndex) const
{
	SetWeaponRecoilStrength(DefaultRecoilStrength, BehaviourIndex);
}

void AWeaponFirearm::ResetAimWeaponSpread(const int32 BehaviourIndex) const
{
	const UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->ResetSpread(WeaponFirearmData.SpreadChangeRate);
}

void AWeaponFirearm::AttachBarrelToSocket() const
{
	if (!IsValid(WeaponMesh) || !IsValid(MainShootBarrel))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("WeaponMesh or ShootBarrel is not valid"));
		return;
	}

	if (WeaponMesh->DoesSocketExist(WeaponFirearmData.ShootBarrelSocketName))
		MainShootBarrel->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponFirearmData.ShootBarrelSocketName);
	else
		MainShootBarrel->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "None");
}
