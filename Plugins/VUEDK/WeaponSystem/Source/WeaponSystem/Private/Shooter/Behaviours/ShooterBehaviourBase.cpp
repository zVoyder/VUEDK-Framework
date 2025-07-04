// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShooterBehaviourBase.h"
#include "Factories/HandlersFactory.h"
#include "Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

void UShooterBehaviourBase::Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel)
{
	if (!IsValid(InShooter))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter in %s is null."), *GetName());
		return;
	}

	if (!IsValid(InShootBarrel))
	{
		UE_LOG(LogShooter, Error, TEXT("ShootBarrel in %s is null."), *GetName());
		return;
	}

	ShootData = InShootData;
	Shooter = InShooter;
	ShootBarrel = InShootBarrel;
	ShootPoints = ShootBarrel->GetShootPointsChildren();
	CreateHandlers();
	EnableBehaviour();
	OnInit();
}

void UShooterBehaviourBase::CreateHandlers()
{
	RecoilHandler = UHandlersFactory::CreateRecoilHandler(this, ShootData);
	CooldownHandler = UHandlersFactory::CreateCooldownHandler(this, ShootData);
	SpreadHandler = UHandlersFactory::CreateSpreadHandler(this, ShootData);
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
	OnEnabled();
	OnBehaviourEnabled.Broadcast();
}

void UShooterBehaviourBase::DisableBehaviour()
{
	if (!IsBehaviourActive())
		return;

	bIsBehaviourActive = false;
	OnDisabled();
	OnBehaviourDisabled.Broadcast();
}

bool UShooterBehaviourBase::Shoot()
{
	if (!Check())
	{
		UE_LOG(LogShooter, Error, TEXT("Shoot() in %s failed check."), *GetName());
		ShootFail(EShootFailReason::Error);
		return false;
	}

	if (!IsBehaviourActive())
	{
		UE_LOG(LogShooter, Warning, TEXT("Shoot() in %s is not active."), *GetName());
		ShootFail(EShootFailReason::Disabled);
		return false;
	}

	if (ShootPoints.Num() <= 0)
	{
		UE_LOG(LogShooter, Error, TEXT("ShootPoints in %s is empty."), *GetName());
		ShootFail(EShootFailReason::NoShootPoints);
		return false;
	}

	if (CooldownHandler->IsInCooldown())
	{
		ShootFail(EShootFailReason::CoolDown);
		return false;
	}

	if (!TryConsumeAmmoForShoot())
	{
		ShootFail(EShootFailReason::NoAmmo);
		return false;
	}

	if (!OnShootCondition(ShootBarrel))
	{
		ShootFail(EShootFailReason::Condition);
		return false;
	}

	HandleShoot();
	ShootSuccess();
	return true;
}

void UShooterBehaviourBase::ResetCooldown() const
{
	CooldownHandler->ResetCooldown();
}

void UShooterBehaviourBase::EndShootSequence()
{
	if (ShotsFired <= 0)
		return;
	
	ShotsFired = 0;
	OnEndShootSequence.Broadcast();
}

void UShooterBehaviourBase::ResetSpread(const float ChangeRate) const
{
	SpreadHandler->ResetSpread(ChangeRate);
}

int32 UShooterBehaviourBase::Refill(const int32 Ammo)
{
	ModifyCurrentAmmo(Ammo);
	OnRefill();
	OnBehaviourRefill.Broadcast(CurrentAmmo);
	return FMath::Abs(CurrentAmmo - Ammo);
}

void UShooterBehaviourBase::RefillAllMagazine()
{
	Refill(GetMagSize());
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
	ShootData.Damage = FMath::Clamp(NewDamage, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetFireRate(const float NewFireRate)
{
	ShootData.FireRate = FMath::Clamp(NewFireRate, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetMaxRange(const float NewRange)
{
	ShootData.MaxRange = FMath::Clamp(NewRange, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetMagSize(const int32 NewMagSize)
{
	ShootData.MagSize = FMath::Clamp(NewMagSize, 0, MAX_int32);
}

void UShooterBehaviourBase::SetRecoilStrength(float NewRecoilStrength)
{
	NewRecoilStrength = FMath::Clamp(NewRecoilStrength, 0.f, 1.f);
	ShootData.RecoilStrength = NewRecoilStrength;
}

void UShooterBehaviourBase::InstantSetSpread(const float InSpread, const bool bOverrideDefault) const
{
	SpreadHandler->InstantSetSpread(InSpread, bOverrideDefault);
}

void UShooterBehaviourBase::SetSpread(const float NewSpread, const float NewSpreadChangeRate, const bool bOverrideDefault) const
{
	SpreadHandler->SetSpread(NewSpread, NewSpreadChangeRate, bOverrideDefault);
}

void UShooterBehaviourBase::SetShootType(const EShootType NewShootType)
{
	ShootData.ShootType = NewShootType;
}

void UShooterBehaviourBase::SetCurrentAmmo(const int32 NewAmmo)
{
	const int32 MagSize = GetMagSize();
	CurrentAmmo = FMath::Clamp(NewAmmo, 0, MagSize);
	OnCurrentAmmoChanged.Broadcast(CurrentAmmo, MagSize);
}

void UShooterBehaviourBase::ModifyCurrentAmmo(const int32 AmmoValue)
{
	SetCurrentAmmo(CurrentAmmo + AmmoValue);
}

bool UShooterBehaviourBase::IsBehaviourActive() const
{
	return bIsBehaviourActive;
}

APawn* UShooterBehaviourBase::GetOwner() const
{
	return Owner;
}

float UShooterBehaviourBase::GetMaxRange_Implementation() const
{
	return ShootData.MaxRange;
}

float UShooterBehaviourBase::GetFireRate_Implementation() const
{
	return ShootData.FireRate;
}

float UShooterBehaviourBase::GetDamage_Implementation() const
{
	return ShootData.Damage;
}

int32 UShooterBehaviourBase::GetMagSize_Implementation() const
{
	return ShootData.MagSize;
}

float UShooterBehaviourBase::GetRecoilStrength_Implementation() const
{
	return ShootData.RecoilStrength;
}

float UShooterBehaviourBase::GetSpread() const
{
	return SpreadHandler->GetSpread();
}

UShootBarrel* UShooterBehaviourBase::GetShootBarrel() const
{
	return ShootBarrel;
}

EShootType UShooterBehaviourBase::GetShootType() const
{
	return ShootData.ShootType;
}

int32 UShooterBehaviourBase::GetShotsFired() const
{
	return ShotsFired;
}

int32 UShooterBehaviourBase::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 UShooterBehaviourBase::GetAmmoToConsume() const
{
	return AmmoToConsumePerShot;
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

bool UShooterBehaviourBase::HasJustShot() const
{
	return bHasJustShot;
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

void UShooterBehaviourBase::HandleShoot()
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
	}
}

void UShooterBehaviourBase::DeployShoot(UShootPoint* ShootPoint) const
{
	const FVector ShooterTargetLocation = GetShooterTargetLocation();
	const FVector DirectionToTarget = (ShooterTargetLocation - ShootPoint->GetShootPointLocation()).GetSafeNormal();
	ShootPoint->GenerateSpreadDegree(GetSpread());
	const FVector DirectionToTargetSpreaded = DirectionToTarget + FMath::VRandCone(DirectionToTarget, ShootPoint->GetSpreadRad());
	OnDeployShoot(ShootPoint, ShooterTargetLocation, DirectionToTarget, DirectionToTargetSpreaded);
}

void UShooterBehaviourBase::ShootSuccess()
{
	if (!IsValid(Shooter))
	{
		UE_LOG(LogShooter, Error, TEXT("ShootSuccess::Shooter in %s is null."), *GetName());
		return;
	}

	OnShootSuccess(ShootBarrel);
	OnBehaviourShootSuccess.Broadcast(ShootBarrel, ShotsFired);
	CooldownHandler->StartCooldown();
	RecoilHandler->ApplyRecoilImpulse();
	SpreadHandler->AddDynamicSpreadWithCurve();
	ShotsFired++;
	CheckMagEmpty();
	TriggerHasJustShot();
}

void UShooterBehaviourBase::ShootFail(const EShootFailReason FailReason)
{
	OnShootFail(FailReason);
	OnBehaviourShootFail.Broadcast();
}

void UShooterBehaviourBase::TickBehaviour(const float DeltaTime)
{
	OnTickBehaviour(DeltaTime);
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

void UShooterBehaviourBase::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& DirectionToTargetSpreaded) const
{
}

void UShooterBehaviourBase::OnShootSuccess_Implementation(const UShootBarrel* OutShootBarrel)
{
}

void UShooterBehaviourBase::OnShootFail_Implementation(const EShootFailReason FailReason)
{
}

bool UShooterBehaviourBase::OnShootCondition_Implementation(UShootBarrel* OutShootBarrel) const
{
	return true;
}

void UShooterBehaviourBase::OnRefill_Implementation()
{
}


void UShooterBehaviourBase::OnMagEmptied_Implementation()
{
}

FVector UShooterBehaviourBase::GetShooterTargetLocation_Implementation() const
{
	FRotator Rotation;
	if (FVector CameraStartPoint, CameraEndPoint, CameraHitPoint; TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint, Rotation))
		return CameraHitPoint;

	return FVector::ZeroVector;
}

bool UShooterBehaviourBase::TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, const FVector StartPointOffset) const
{
	const UWorld* World = Shooter->GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour LineTraceFromCamera(), World is invalid in %s."), *GetName());
		return false;
	}

	const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
	if (!IsValid(CameraManager))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour LineTraceFromCamera(), CameraManager is null in %s."), *GetName());
		return false;
	}

	const FVector CameraLocation = CameraManager->GetCameraCacheView().Location;
	OutRotation = CameraManager->GetCameraCacheView().Rotation;

	const FVector WorldOffset = OutRotation.RotateVector(StartPointOffset);
	OutStartPoint = CameraLocation + WorldOffset;
	OutEndPoint = OutStartPoint + OutRotation.Vector() * GetMaxRange();
	OutHitPoint = OutEndPoint;

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, OutStartPoint, OutEndPoint, SightTraceChannel))
		OutHitPoint = HitResult.ImpactPoint;

	return true;
}

bool UShooterBehaviourBase::IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour IsInLineOfSight(), World is invalid in %s."), *GetName());
		return false;
	}

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, StartPoint, TargetPoint, SightTraceChannel))
		return HitResult.ImpactPoint.Equals(TargetPoint, Tolerance);

	return true;
}

bool UShooterBehaviourBase::Check() const
{
	return IsValid(Shooter) && IsValid(ShootBarrel);
}

void UShooterBehaviourBase::HandleSimultaneousShoot()
{
	for (UShootPoint* ShootPoint : ShootPoints)
	{
		if (!IsValid(ShootPoint))
		{
			UE_LOG(LogShooter, Error, TEXT("HandleSimultaneousShoot(), Invalid ShootPoint in %s."), *GetName());
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
		UE_LOG(LogShooter, Error, TEXT("HandleSequentialShoot(), Invalid ShootPoint in %s."), *GetName());
		return;
	}

	DeployShoot(ShootPoints[CurrentShootPointIndex]);
}

int32 UShooterBehaviourBase::NextShootPointIndex()
{
	if (ShootPoints.Num() == 0)
	{
		UE_LOG(LogShooter, Error, TEXT("ShootPoints in %s is empty."), *GetName());
		return -1;
	}

	const int32 ShootPointIndex = (CurrentShootPointIndex + 1) % ShootPoints.Num();
	CurrentShootPointIndex = ShootPointIndex;
	return CurrentShootPointIndex;
}

bool UShooterBehaviourBase::TryConsumeAmmoForShoot()
{
	if (!HasEnoughAmmoToShoot())
		return false;

	ModifyCurrentAmmo(-GetAmmoToConsume());
	return true;
}

bool UShooterBehaviourBase::HasEnoughAmmoToShoot() const
{
	return bHasInfiniteAmmo || CurrentAmmo - GetAmmoToConsume() >= 0;
}

void UShooterBehaviourBase::CheckMagEmpty()
{
	if (!HasEnoughAmmoToShoot())
	{
		OnMagEmptied();
		OnMagEmpty.Broadcast();
	}
}

void UShooterBehaviourBase::TriggerHasJustShot()
{
	if (bHasJustShot)
		return;

	bHasJustShot = true;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UShooterBehaviourBase::NextTickHasJustShot);
}

void UShooterBehaviourBase::NextTickHasJustShot()
{
	if (HasJustShotTickCount >= HasJustShootTicks)
	{
		ResetHasJustShot();
		return;
	}

	HasJustShotTickCount++;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UShooterBehaviourBase::NextTickHasJustShot);
}

void UShooterBehaviourBase::ResetHasJustShot()
{
	bHasJustShot = false;
	HasJustShotTickCount = 0;
}
