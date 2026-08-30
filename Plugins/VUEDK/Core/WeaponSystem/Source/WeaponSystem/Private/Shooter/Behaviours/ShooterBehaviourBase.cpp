// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShooterBehaviourBase.h"
#include "WeaponSystem.h"
#include "Factories/HandlersFactory.h"
#include "Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Data/WeaponShootData.h"

void UShooterBehaviourBase::Init(UShooter* InShooter)
{
	if (!IsValid(InShooter))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterBehaviourBase::Init: Shooter in %s is null."), *GetName());
		return;
	}

	if (!IsValid(ShootData))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterBehaviourBase::Init: ShootData in %s is null."), *GetName());
		return;
	}

	Shooter = InShooter;
	CreateHandlers();
	ResetAll();
	EnableBehaviour();
	OnInit();
}

void UShooterBehaviourBase::SetupShootBarrel(UShootBarrel* InShootBarrel)
{
	if (!IsValid(InShootBarrel))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterBehaviourBase::SetupShootBarrel: InShootBarrel in %s is null."), *GetName());
		return;
	}

	ShootBarrel = InShootBarrel;
	ShootPoints = ShootBarrel->GetShootPointsChildren();
}

void UShooterBehaviourBase::CreateHandlers()
{
	RecoilHandler = UHandlersFactory::CreateHandler<URecoilHandler>(this);
	CooldownHandler = UHandlersFactory::CreateHandler<UCooldownHandler>(this);
	SpreadHandler = UHandlersFactory::CreateHandler<USpreadHandler>(this);
	ShootModesHandler = UHandlersFactory::CreateHandler<UShootModesHandler>(this);
	ShootModesHandler->OnShootRequestHandled.AddDynamic(this, &UShooterBehaviourBase::OnHandleShootRequest);
	ShootModesHandler->OnShootSequenceEnded.AddDynamic(this, &UShooterBehaviourBase::CallShootSequenceEndEvent);
}

void UShooterBehaviourBase::SetOwner(APawn* InOwner)
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogShooter, Error, TEXT("Owner in %s is null."), *GetName());
		return;
	}

	Owner = InOwner;
}

void UShooterBehaviourBase::SetBehaviourEnabled(const bool bEnabled)
{
	if (bEnabled)
		EnableBehaviour();
	else
		DisableBehaviour();
}

void UShooterBehaviourBase::EnableBehaviour()
{
	if (IsBehaviourActive())
		return;

	bIsBehaviourActive = true;
	CallEnableEvent();
}

void UShooterBehaviourBase::DisableBehaviour()
{
	if (!IsBehaviourActive())
		return;

	bIsBehaviourActive = false;
	CallDisableEvent();
}

bool UShooterBehaviourBase::Shoot()
{
	if (!Check())
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::Shoot: in %s failed check."), *GetName());
		ShootFail(EShootFailReason::Error);
		return false;
	}

	if (!IsBehaviourActive())
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::Shoot: in %s is not active."), *GetName());
		ShootFail(EShootFailReason::Disabled);
		return false;
	}

	if (ShootPoints.Num() <= 0)
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::Shoot: ShootPoints in %s is empty."), *GetName());
		ShootFail(EShootFailReason::NoShootPoints);
		return false;
	}

	if (CooldownHandler->IsInCooldown())
	{
		ShootFail(EShootFailReason::CoolDown);
		return false;
	}

	if (!GetShootCondition(ShootBarrel))
	{
		ShootFail(EShootFailReason::Condition);
		return false;
	}

	if (!HasEnoughAmmoToShoot())
	{
		ShootFail(EShootFailReason::NoAmmo);
		return false;
	}

	return ShootModesHandler->RequestShoot(CurrentShootType);
}

void UShooterBehaviourBase::EndShootSequence() const
{
	ShootModesHandler->EndSequence();
}

void UShooterBehaviourBase::ResetSpread(const float ChangeRate) const
{
	SpreadHandler->ResetSpread(ChangeRate);
}

int32 UShooterBehaviourBase::RefillMagazine(const int32 Ammo, int32& OutRemainingAmmo)
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return 0;

	return RelatedMagazine->Refill(Ammo, OutRemainingAmmo, this);
}

void UShooterBehaviourBase::RefillAllMagazine()
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return;

	RelatedMagazine->RefillAllMagazine(this);
}

int32 UShooterBehaviourBase::RefillWithAmmoType(UAmmoTypeData* AmmoType, const int32 Ammo, int32& OutRemainingAmmo)
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return 0;

	return RelatedMagazine->RefillWithAmmoType(AmmoType, Ammo, OutRemainingAmmo, this);
}

void UShooterBehaviourBase::AddDynamicSpread(const float AddSpread, const float ChangeRate, const float RecoveryRate) const
{
	SpreadHandler->AddDynamicSpread(AddSpread, ChangeRate, RecoveryRate);
}

void UShooterBehaviourBase::SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength, const float NewDefaultSpread)
{
	SetDamage(NewDamage);
	SetFireRate(NewFireRate);
	SetMaxRange(NewRange);
	SetMagSize(NewMagSize);
	SetRecoilStrength(NewRecoilStrength);
	InstantSetSpread(NewDefaultSpread, true);
}

void UShooterBehaviourBase::SetDamage(const float NewDamage)
{
	CurrentDamage = FMath::Clamp(NewDamage, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetFireRate(const float NewFireRate)
{
	CurrentFireRate = FMath::Clamp(NewFireRate, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetMaxRange(const float NewRange)
{
	CurrentMaxRange = FMath::Clamp(NewRange, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetMagSize(const int32 NewMagSize) const
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return;

	RelatedMagazine->SetMagazineSize(NewMagSize);
}

void UShooterBehaviourBase::SetRecoilStrength(const float NewRecoilStrength)
{
	CurrentRecoilStrength = FMath::Clamp(NewRecoilStrength, 0.f, 1.f);
}

void UShooterBehaviourBase::InstantSetSpread(const float InSpread, const bool bOverrideDefault) const
{
	SpreadHandler->InstantSetSpread(InSpread, bOverrideDefault);
}

void UShooterBehaviourBase::SetSpread(const float NewSpread, const float NewSpreadChangeRate, const bool bOverrideDefault) const
{
	SpreadHandler->SetSpread(NewSpread, NewSpreadChangeRate, bOverrideDefault);
}

void UShooterBehaviourBase::SetDefaultSpread(const float NewDefaultSpread)
{
	CurrentDefaultSpread = NewDefaultSpread;
}

void UShooterBehaviourBase::SetInfiniteAmmo(const bool bEnabled)
{
	bCurrentInfiniteAmmo = bEnabled;
}

void UShooterBehaviourBase::ChangeShootType(const EShootType NewShootType)
{
	CurrentShootType = NewShootType;
}

void UShooterBehaviourBase::ChangeMagazine(const int32 NewMagazineIndex)
{
	UMagazine* OldMagazine = GetRelatedMagazine();
	UnbindMagazineEvents(OldMagazine);
	CurrentMagazineIndex = NewMagazineIndex;
	UMagazine* NewMagazine = GetRelatedMagazine();
	BindMagazineEvents(NewMagazine);
}

void UShooterBehaviourBase::ChangeShootMode(const int32 NewShootModeIndex) const
{
	ShootModesHandler->SetModeIndex(NewShootModeIndex);
}

bool UShooterBehaviourBase::IsBehaviourActive() const
{
	return bIsBehaviourActive;
}

bool UShooterBehaviourBase::IsShooting() const
{
	return ShootModesHandler->IsProcessingRequest();
}

bool UShooterBehaviourBase::IsMagazineEmpty() const
{
	const UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return false;

	return RelatedMagazine->IsEmpty();
}

bool UShooterBehaviourBase::IsMagazineFull() const
{
	const UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return false;

	return RelatedMagazine->IsFull();
}

bool UShooterBehaviourBase::UsesAmmoOfType(const UAmmoTypeData* InAmmoType) const
{
	const UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return false;

	return RelatedMagazine->IsOfAmmoType(InAmmoType);
}

bool UShooterBehaviourBase::HasInfiniteAmmo() const
{
	return bCurrentInfiniteAmmo;
}

bool UShooterBehaviourBase::HasEnoughAmmoToShoot() const
{
	if (HasInfiniteAmmo())
		return true;

	if (!Check())
		return false;

	const UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return false;

	const int32 AmmoPerShot = ShootData->AmmoToConsumePerShot;
	return RelatedMagazine->CanConsumeAmmo(AmmoPerShot);
}

UShooter* UShooterBehaviourBase::GetShooter() const
{
	return Shooter;
}

APawn* UShooterBehaviourBase::GetOwner() const
{
	return Owner;
}

float UShooterBehaviourBase::GetDamage() const
{
	return CurrentDamage;
}

float UShooterBehaviourBase::GetMaxRange() const
{
	return CurrentMaxRange;
}

float UShooterBehaviourBase::GetFireRate() const
{
	return CurrentFireRate;
}

int32 UShooterBehaviourBase::GetMagazineSize() const
{
	const UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return -1;

	return RelatedMagazine->GetMagazineSize();
}

int32 UShooterBehaviourBase::GetMagazineCurrentAmmo() const
{
	const UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return -1;

	return RelatedMagazine->GetCurrentAmmo();
}

float UShooterBehaviourBase::GetRecoilStrength() const
{
	return CurrentRecoilStrength;
}

float UShooterBehaviourBase::GetSpread() const
{
	return SpreadHandler->GetSpread();
}

float UShooterBehaviourBase::GetDefaultSpread() const
{
	return CurrentDefaultSpread;
}

bool UShooterBehaviourBase::TryGetReloadMontageIndex(int32& OutIndex) const
{
	const UWeaponShootData* WeaponShootData = Cast<UWeaponShootData>(ShootData);
	if (!IsValid(WeaponShootData))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::GetReloadAnimIndex_Implementation: ShootData in %s is not of type UWeaponShootData."), *GetName());
		return false;
	}

	if (!WeaponShootData->bEnableReloadMontage)
		return false;

	OutIndex = GetReloadMontageIndex(WeaponShootData);
	if (OutIndex < 0)
		return false;
	
	return true;
}

bool UShooterBehaviourBase::TryGetShootSuccessMontageIndex(int32& OutIndex, const int32 ShotIndex) const
{
	const UWeaponShootData* WeaponShootData = Cast<UWeaponShootData>(ShootData);
	if (!IsValid(WeaponShootData))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::GetShootSuccessMontageIndexNative: ShootData in %s is not of type UWeaponShootData."), *GetName());
		return false;
	}

	if (!WeaponShootData->bEnableShootSuccessMontage)
		return false;

	OutIndex = GetShootSuccessMontageIndex(WeaponShootData, ShotIndex);
	if (OutIndex < 0)
		return false;
	
	return true;
}

bool UShooterBehaviourBase::TryGetShootFailMontageIndex(int32& OutIndex, const EShootFailReason FailReason) const
{
	const UWeaponShootData* WeaponShootData = Cast<UWeaponShootData>(ShootData);
	if (!IsValid(WeaponShootData))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::GetShootFailMontageIndexNative: ShootData in %s is not of type UWeaponShootData."), *GetName());
		return false;
	}

	if (!WeaponShootData->bEnableShootFailMontage)
		return false;

	OutIndex = GetShootFailMontageIndex(WeaponShootData, FailReason);
	if (OutIndex < 0)
		return false;
	
	return true;
}

UShootBarrel* UShooterBehaviourBase::GetShootBarrel() const
{
	return ShootBarrel;
}

EShootType UShooterBehaviourBase::GetShootType() const
{
	return CurrentShootType;
}

UMagazine* UShooterBehaviourBase::GetRelatedMagazine() const
{
	if (!IsValid(Shooter))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::GetRelatedMagazine: Shooter in %s is null."), *GetName());
		return nullptr;
	}

	return Shooter->GetMagazine(CurrentMagazineIndex);
}

int32 UShooterBehaviourBase::GetShootModeIndex() const
{
	return ShootModesHandler->GetModeIndex();
}

UShootModeBase* UShooterBehaviourBase::GetShootMode() const
{
	return ShootModesHandler->GetShootMode();
}

TEnumAsByte<ECollisionChannel> UShooterBehaviourBase::GetSightTraceChannel() const
{
	if (!IsValid(ShootData))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::GetSightTraceChannel: ShootData in %s is null."), *GetName());
		return ECollisionChannel::ECC_Visibility;
	}

	return ShootData->SightTraceChannel;
}

TSubclassOf<UDamageType> UShooterBehaviourBase::GetDamageTypeClass() const
{
	if (!IsValid(ShootData))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::GetDamageTypeClass: ShootData in %s is null."), *GetName());
		return UDamageType::StaticClass();
	}

	return ShootData->DamageTypeClass;
}

int32 UShooterBehaviourBase::GetShootHandledRequestsCount() const
{
	return ShootModesHandler->GetHandledRequests();
}

UCooldownHandler* UShooterBehaviourBase::GetCooldownHandler() const
{
	return CooldownHandler;
}

URecoilHandler* UShooterBehaviourBase::GetRecoilHandler() const
{
	return RecoilHandler;
}

USpreadHandler* UShooterBehaviourBase::GetSpreadHandler() const
{
	return SpreadHandler;
}

void UShooterBehaviourBase::ResetAll()
{
	ResetCooldown();
	ResetDamage();
	ResetFireRate();
	ResetMaxRange();
	ResetRecoilStrength();
	ResetDefaultSpread();
	ResetInfiniteAmmo();
	ResetShootType();
	ResetMagazine();
}

void UShooterBehaviourBase::ResetCooldown() const
{
	CooldownHandler->ResetCooldown();
}

void UShooterBehaviourBase::ResetDamage()
{
	CurrentDamage = ShootData->Damage;
}

void UShooterBehaviourBase::ResetFireRate()
{
	CurrentFireRate = ShootData->FireRate;
}

void UShooterBehaviourBase::ResetMaxRange()
{
	CurrentMaxRange = ShootData->MaxRange;
}

void UShooterBehaviourBase::ResetRecoilStrength()
{
	CurrentRecoilStrength = ShootData->RecoilStrength;
}

void UShooterBehaviourBase::ResetDefaultSpread()
{
	if (!Check())
		return;

	CurrentDefaultSpread = ShootData->DefaultSpread;
}

void UShooterBehaviourBase::ResetInfiniteAmmo()
{
	bCurrentInfiniteAmmo = ShootData->bHasInfiniteAmmo;
}

void UShooterBehaviourBase::ResetShootType()
{
	CurrentShootType = ShootData->ShootType;
}

void UShooterBehaviourBase::ResetMagazine()
{
	ChangeMagazine(ShootData->MagazineIndex);
}

UWorld* UShooterBehaviourBase::GetWorld() const
{
	if (!IsValid(Shooter))
		return Super::GetWorld();

	return Shooter->GetWorld();
}

#if WITH_EDITOR
bool UShooterBehaviourBase::ImplementsGetWorld() const
{
	// Return true so in Editor we can see WorldContexts Functions
	return true;
}
#endif

void UShooterBehaviourBase::DeployShootOfType()
{
	switch (GetShootType())
	{
	case EShootType::Simultaneous:
		{
			HandleSimultaneousShoot();
			break;
		}

	case EShootType::Sequential:
		{
			HandleSequentialShoot();
			break;
		}
	default: ;
	}
}

bool UShooterBehaviourBase::TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, const FVector StartPointOffset) const
{
	if (!Check())
	{
		UE_LOG(LogShooter, Warning, TEXT("ShooterBehaviour TryGetCameraPoints() failed check in %s."), *GetName());
		return false;
	}

	const UWorld* World = Shooter->GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Warning, TEXT("ShooterBehaviour LineTraceFromCamera(), World is invalid in %s."), *GetName());
		return false;
	}

	const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
	if (!IsValid(CameraManager))
	{
		UE_LOG(LogShooter, Warning, TEXT("ShooterBehaviour LineTraceFromCamera(), CameraManager is null in %s."), *GetName());
		return false;
	}

	const FVector CameraLocation = CameraManager->GetCameraCacheView().Location;
	OutRotation = CameraManager->GetCameraCacheView().Rotation;

	const FVector WorldOffset = OutRotation.RotateVector(StartPointOffset);
	OutStartPoint = CameraLocation + WorldOffset;
	OutEndPoint = OutStartPoint + OutRotation.Vector() * GetMaxRange();
	OutHitPoint = OutEndPoint;

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, OutStartPoint, OutEndPoint, GetSightTraceChannel()))
		OutHitPoint = HitResult.ImpactPoint;

	return true;
}

bool UShooterBehaviourBase::IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance) const
{
	if (!Check())
	{
		UE_LOG(LogShooter, Warning, TEXT("ShooterBehaviour IsInLineOfSight() failed check in %s."), *GetName());
		return false;
	}

	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Warning, TEXT("ShooterBehaviour IsInLineOfSight(), World is invalid in %s."), *GetName());
		return false;
	}

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, StartPoint, TargetPoint, GetSightTraceChannel()))
		return HitResult.ImpactPoint.Equals(TargetPoint, Tolerance);

	return true;
}

void UShooterBehaviourBase::DeployShoot(UShootPoint* ShootPoint) const
{
	ShootPoint->GenerateSpreadDegree(GetSpread());
	const FVector ShooterTargetLocation = GetShooterTargetLocation();
	const FVector DirectionToTarget = (ShooterTargetLocation - ShootPoint->GetShootPointLocation()).GetSafeNormal();
	const FVector ShootPointDirection = ShootPoint->GetShootPointDirection();
	const FVector SelectDirection = bUseDirectionToTarget ? DirectionToTarget : ShootPointDirection;
	const FVector ShootDirection = SelectDirection + FMath::VRandCone(SelectDirection, ShootPoint->GetSpreadRad());
	OnDeployShoot(ShootPoint, ShooterTargetLocation, DirectionToTarget, ShootDirection);
}

void UShooterBehaviourBase::ShootSuccess(const int32 RequestIndex)
{
	if (!IsValid(Shooter))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::ShootSuccess: Shooter in %s is null."), *GetName());
		return;
	}

	CallShootSuccessEvent(RequestIndex);
	CooldownHandler->StartCooldown();
	RecoilHandler->ApplyRecoilImpulse();
	SpreadHandler->AddDynamicSpreadWithCurve();
}

void UShooterBehaviourBase::ShootFail(const EShootFailReason FailReason)
{
	CallShootFailEvent(FailReason);
}

void UShooterBehaviourBase::TickBehaviour(const float DeltaTime)
{
	TickHandlers(DeltaTime);
	OnTickBehaviour(DeltaTime);
}

void UShooterBehaviourBase::TickHandlers(const float DeltaTime) const
{
	if (IsValid(CooldownHandler))
		CooldownHandler->TickHandler(DeltaTime);

	if (IsValid(RecoilHandler))
		RecoilHandler->TickHandler(DeltaTime);

	if (IsValid(SpreadHandler))
		SpreadHandler->TickHandler(DeltaTime);
}

void UShooterBehaviourBase::OnInit_Implementation()
{
}

void UShooterBehaviourBase::OnTickBehaviour_Implementation(const float DeltaTime)
{
}

void UShooterBehaviourBase::OnEnabled_Implementation()
{
}

void UShooterBehaviourBase::OnDisabled_Implementation()
{
}

FVector UShooterBehaviourBase::GetShooterTargetLocation_Implementation() const
{
	FRotator Rotation;
	if (FVector CameraStartPoint, CameraEndPoint, CameraHitPoint; TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint, Rotation))
		return CameraHitPoint;

	return FVector::ZeroVector;
}

int32 UShooterBehaviourBase::GetReloadMontageIndex_Implementation(const UWeaponShootData* WeaponShootData) const
{
	return WeaponShootData->ReloadMontageIndex;
}

int32 UShooterBehaviourBase::GetShootSuccessMontageIndex_Implementation(const UWeaponShootData* WeaponShootData, const int32 ShotIndex) const
{
	return WeaponShootData->ShootSuccessMontageIndex;
}

int32 UShooterBehaviourBase::GetShootFailMontageIndex_Implementation(const UWeaponShootData* WeaponShootData, const EShootFailReason FailReason) const
{
	return WeaponShootData->ShootFailMontageIndex;
}

void UShooterBehaviourBase::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& ShootDirection) const
{
}

void UShooterBehaviourBase::OnShootSequenceEnd_Implementation()
{
}

void UShooterBehaviourBase::OnShootSuccess_Implementation(const UShootBarrel* OutShootBarrel)
{
}

void UShooterBehaviourBase::OnShootFail_Implementation(const EShootFailReason FailReason)
{
}

bool UShooterBehaviourBase::GetShootCondition_Implementation(UShootBarrel* OutShootBarrel) const
{
	return true;
}

void UShooterBehaviourBase::OnAmmoChange_Implementation(UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize)
{
}

void UShooterBehaviourBase::OnRefill_Implementation(UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
}

void UShooterBehaviourBase::OnFull_Implementation(UMagazine* Magazine)
{
}

void UShooterBehaviourBase::OnEmpty_Implementation(UMagazine* Magazine)
{
}

bool UShooterBehaviourBase::Check() const
{
	if (!IsValid(Shooter))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::Check: ShooterBehaviour %s has null Shooter."), *GetName());
		return false;
	}

	if (!IsValid(ShootData))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::Check: ShooterBehaviour %s has null ShootData."), *GetName());
		return false;
	}

	if (!IsValid(ShootBarrel))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::Check: ShooterBehaviour %s has not setup a ShootBarrel."), *GetName());
		return false;
	}

	return IsValid(CooldownHandler) && IsValid(RecoilHandler) && IsValid(SpreadHandler);
}

void UShooterBehaviourBase::HandleSimultaneousShoot()
{
	for (UShootPoint* ShootPoint : ShootPoints)
	{
		if (!IsValid(ShootPoint))
		{
			UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::HandleSimultaneousShoot: Invalid ShootPoint in %s."), *GetName());
			continue;
		}

		DeployShoot(ShootPoint);
	}
}

void UShooterBehaviourBase::HandleSequentialShoot()
{
	NextShootPointIndex();

	if (!IsValid(ShootPoints[CurrentShootPointIndex]))
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::HandleSequentialShoot: Invalid ShootPoint in %s."), *GetName());
		return;
	}

	DeployShoot(ShootPoints[CurrentShootPointIndex]);
}

int32 UShooterBehaviourBase::NextShootPointIndex()
{
	if (ShootPoints.Num() == 0)
	{
		UE_LOG(LogShooter, Warning, TEXT("UShooterBehaviourBase::NextShootPointIndex: ShootPoints in %s is empty."), *GetName());
		return -1;
	}

	const int32 ShootPointIndex = (CurrentShootPointIndex + 1) % ShootPoints.Num();
	CurrentShootPointIndex = ShootPointIndex;
	return CurrentShootPointIndex;
}

bool UShooterBehaviourBase::TryConsumeAmmoForShoot()
{
	if (HasInfiniteAmmo())
		return true;

	if (!Check())
		return false;

	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (!IsValid(RelatedMagazine))
		return false;

	const int32 AmmoPerShot = ShootData->AmmoToConsumePerShot;
	return RelatedMagazine->TryConsumeAmmo(AmmoPerShot, this);
}

void UShooterBehaviourBase::CallEnableEvent()
{
	OnEnabled();
	OnBehaviourEnabled.Broadcast(this);
}

void UShooterBehaviourBase::CallDisableEvent()
{
	OnDisabled();
	OnBehaviourDisabled.Broadcast(this);
}

void UShooterBehaviourBase::CallShootSequenceEndEvent()
{
	OnShootSequenceEnd();
	OnBehaviourShootSequenceEnded.Broadcast(this);
}

void UShooterBehaviourBase::CallShootSuccessEvent(const int32 RequestIndex)
{
	OnShootSuccess(ShootBarrel);
	OnBehaviourShootSuccess.Broadcast(this, ShootBarrel, RequestIndex);
}

void UShooterBehaviourBase::CallShootFailEvent(const EShootFailReason FailReason)
{
	OnShootFail(FailReason);
	OnBehaviourShootFail.Broadcast(this, FailReason);
}

void UShooterBehaviourBase::CallAmmoChangeEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize)
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (Magazine != RelatedMagazine)
		return;
	
	OnAmmoChange(RelatedMagazine, CurrentAmmo, MagSize);
	OnBehaviourAmmoChanged.Broadcast(this, RelatedMagazine, CurrentAmmo, MagSize);
}

void UShooterBehaviourBase::CallRefillEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (Magazine != RelatedMagazine)
		return;
	
	OnRefill(RelatedMagazine, CurrentAmmo, RefilledAmmo, RemainingAmmo);
	OnBehaviourRefilled.Broadcast(this, RelatedMagazine, CurrentAmmo, RefilledAmmo, RemainingAmmo);
}

void UShooterBehaviourBase::CallFullEvent(const UObject* Instigator, const UMagazine* Magazine)
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (Magazine != RelatedMagazine)
		return;
	
	OnFull(RelatedMagazine);
	OnBehaviourFull.Broadcast(this, RelatedMagazine);
}

void UShooterBehaviourBase::CallEmptyEvent(const UObject* Instigator, const UMagazine* Magazine)
{
	UMagazine* RelatedMagazine = GetRelatedMagazine();
	if (Magazine != RelatedMagazine)
		return;

	OnEmpty(RelatedMagazine);
	OnBehaviourEmpty.Broadcast(this, RelatedMagazine);
}

void UShooterBehaviourBase::BindMagazineEvents(UMagazine* Magazine)
{
	if (!IsValid(Magazine))
		return;

	Magazine->OnMagazineAmmoChanged.AddDynamic(this, &UShooterBehaviourBase::CallAmmoChangeEvent);
	Magazine->OnMagazineRefilled.AddDynamic(this, &UShooterBehaviourBase::CallRefillEvent);
	Magazine->OnMagazineFull.AddDynamic(this, &UShooterBehaviourBase::CallFullEvent);
	Magazine->OnMagazineEmpty.AddDynamic(this, &UShooterBehaviourBase::CallEmptyEvent);
}

void UShooterBehaviourBase::UnbindMagazineEvents(UMagazine* Magazine)
{
	if (!IsValid(Magazine))
		return;

	Magazine->OnMagazineAmmoChanged.RemoveDynamic(this, &UShooterBehaviourBase::CallAmmoChangeEvent);
	Magazine->OnMagazineRefilled.RemoveDynamic(this, &UShooterBehaviourBase::CallRefillEvent);
	Magazine->OnMagazineFull.RemoveDynamic(this, &UShooterBehaviourBase::CallFullEvent);
	Magazine->OnMagazineEmpty.RemoveDynamic(this, &UShooterBehaviourBase::CallEmptyEvent);
}

void UShooterBehaviourBase::OnHandleShootRequest(int32 RequestIndex, UShootModeBase* ShootMode, bool bDeployShoot, bool bSuccess)
{
	if (!bSuccess)
	{
		ShootFail(EShootFailReason::ShootMode);
		return;
	}

	if (!TryConsumeAmmoForShoot())
	{
		ShootFail(EShootFailReason::NoAmmo);
		return;
	}
	
	if (bDeployShoot)
		DeployShootOfType();

	ShootSuccess(RequestIndex);
}
