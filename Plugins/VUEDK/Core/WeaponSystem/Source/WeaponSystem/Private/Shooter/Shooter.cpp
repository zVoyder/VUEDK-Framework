// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Shooter.h"
#include "WeaponSystem.h"

UShooter::UShooter()
{
	PrimaryComponentTick.bCanEverTick = true;
	MagazinesManager = CreateDefaultSubobject<UMagazinesManager>(TEXT("MagazinesManager"));
}

void UShooter::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickBehaviours(DeltaTime);
}

void UShooter::Init(APawn* InOwner)
{
	MagazinesManager->Init(this);

	for (UShooterBehaviourBase* Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->SetOwner(InOwner);
		Behaviour->Init(this);
	}

	BindEvents();
}

void UShooter::SetupShootBarrel(UShootBarrel* InShootBarrel, const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* Behaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(Behaviour))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::SetupShootBarrel: Invalid BehaviourIndex %d."), BehaviourIndex);
		return;
	}

	Behaviour->SetupShootBarrel(InShootBarrel);
}

bool UShooter::Shoot(const int32 BehaviourIndex) const
{
	if (!bCanShootInParallel && IsAnyBehaviourShooting(BehaviourIndex))
	{
		UE_LOG(LogShooter, Warning, TEXT("Shooter::Shoot: Cannot shoot in parallel, another behaviour is already shooting."));
		return false;
	}

	UShooterBehaviourBase* ShooterBehaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(ShooterBehaviour))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::Shoot: Invalid BehaviourIndex %d."), BehaviourIndex);
		return false;
	}

	return ShooterBehaviour->Shoot();
}

void UShooter::ShootAll() const
{
	if (!bCanShootInParallel && IsAnyBehaviourShooting())
	{
		UE_LOG(LogShooter, Warning, TEXT("Shooter::ShootAll: Cannot shoot in parallel, another behaviour is already shooting."));
		return;
	}

	for (const auto Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->Shoot();
	}
}

void UShooter::EndShootSequence(const int32 BehaviourIndex) const
{
	UShooterBehaviourBase* ShooterBehaviour = GetShooterBehaviour(BehaviourIndex);
	if (!IsValid(ShooterBehaviour))
		return;

	ShooterBehaviour->EndShootSequence();
}

void UShooter::EnableAllBehaviours() const
{
	for (const auto Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->EnableBehaviour();
	}
}

void UShooter::DisableAllBehaviours() const
{
	for (const auto Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->DisableBehaviour();
	}
}

UShooterBehaviourBase* UShooter::GetShooterBehaviour(const int32 BehaviourIndex) const
{
	if (!ShooterBehaviours.IsValidIndex(BehaviourIndex))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::GetSecondaryShooterBehaviour: Invalid BehaviourIndex %d."), BehaviourIndex);
		return nullptr;
	}

	return ShooterBehaviours[BehaviourIndex];
}

UMagazine* UShooter::GetMagazine(const int32 MagazineIndex) const
{
	if (!IsValid(MagazinesManager))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::GetMagazineByTag: MagazinesManager is not valid."));
		return nullptr;
	}

	return MagazinesManager->GetMagazine(MagazineIndex);
}

bool UShooter::IsAnyBehaviourShooting(const int32 IgnoreBehaviourIndex) const
{
	for (int32 i = 0; i < ShooterBehaviours.Num(); ++i)
	{
		if (i == IgnoreBehaviourIndex)
			continue;

		const UShooterBehaviourBase* Behaviour = ShooterBehaviours[i];
		if (!IsValid(Behaviour))
			continue;

		if (Behaviour->IsShooting())
			return true;
	}

	return false;
}

void UShooter::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoInitialize)
		AutoInit();
}

void UShooter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnbindEvents();
	DisableAllBehaviours();
}

void UShooter::AutoInit()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwnerPawn))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter::BeginPlay: Owner is not a Pawn."));
		return;
	}

	TArray<UShootBarrel*> ShootBarrels;
	OwnerPawn->GetComponents<UShootBarrel>(ShootBarrels);
	
	if (ShootBarrels.IsEmpty())
	{
		UE_LOG(LogShooter, Warning, TEXT("Shooter::BeginPlay: No ShootBarrel found on Owner Pawn."));
		return;
	}
	
	for (int32 i = 0; i < ShooterBehaviours.Num(); ++i)
	{
		if (!IsValid(ShooterBehaviours[i]))
			continue;

		if (!ShootBarrels.IsValidIndex(i))
		{
			UE_LOG(LogShooter, Warning, TEXT("Shooter::BeginPlay: Not enough ShootBarrels found on Owner Pawn for all ShooterBehaviours."));
			break;
		}

		ShooterBehaviours[i]->SetupShootBarrel(ShootBarrels[i]);
	}

	Init(OwnerPawn);
}

void UShooter::BindEvents() const
{
	for (const auto Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->OnBehaviourEnabled.AddDynamic(this, &UShooter::CallBehaviourEnabledEvent);
		Behaviour->OnBehaviourDisabled.AddDynamic(this, &UShooter::CallBehaviourDisabledEvent);
		Behaviour->OnBehaviourShootSuccess.AddDynamic(this, &UShooter::CallBehaviourShootSuccessEvent);
		Behaviour->OnBehaviourShootFail.AddDynamic(this, &UShooter::CallBehaviourShootFailEvent);
		Behaviour->OnBehaviourShootSequenceEnded.AddDynamic(this, &UShooter::CallBehaviourShootSequenceEndedEvent);
		Behaviour->OnBehaviourAmmoChanged.AddDynamic(this, &UShooter::CallBehaviourAmmoChangedEvent);
		Behaviour->OnBehaviourRefilled.AddDynamic(this, &UShooter::CallBehaviourRefilledEvent);
		Behaviour->OnBehaviourFull.AddDynamic(this, &UShooter::CallBehaviourFullEvent);
		Behaviour->OnBehaviourEmpty.AddDynamic(this, &UShooter::CallBehaviourEmptyEvent);
	}

	for (const auto& Mag : MagazinesManager->Magazines)
	{
		if (!IsValid(Mag))
			continue;

		Mag->OnMagazineRefilled.AddDynamic(this, &UShooter::CallMagazineRefilledEvent);
		Mag->OnMagazineAmmoChanged.AddDynamic(this, &UShooter::CallMagazineAmmoChangedEvent);
		Mag->OnMagazineFull.AddDynamic(this, &UShooter::CallMagazineFullEvent);
		Mag->OnMagazineEmpty.AddDynamic(this, &UShooter::CallMagazineEmptyEvent);
	}
}

void UShooter::UnbindEvents() const
{
	for (const auto Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->OnBehaviourEnabled.RemoveDynamic(this, &UShooter::CallBehaviourEnabledEvent);
		Behaviour->OnBehaviourDisabled.RemoveDynamic(this, &UShooter::CallBehaviourDisabledEvent);
		Behaviour->OnBehaviourShootSuccess.RemoveDynamic(this, &UShooter::CallBehaviourShootSuccessEvent);
		Behaviour->OnBehaviourShootFail.RemoveDynamic(this, &UShooter::CallBehaviourShootFailEvent);
		Behaviour->OnBehaviourShootSequenceEnded.RemoveDynamic(this, &UShooter::CallBehaviourShootSequenceEndedEvent);
		Behaviour->OnBehaviourAmmoChanged.RemoveDynamic(this, &UShooter::CallBehaviourAmmoChangedEvent);
		Behaviour->OnBehaviourRefilled.RemoveDynamic(this, &UShooter::CallBehaviourRefilledEvent);
		Behaviour->OnBehaviourFull.RemoveDynamic(this, &UShooter::CallBehaviourFullEvent);
		Behaviour->OnBehaviourEmpty.RemoveDynamic(this, &UShooter::CallBehaviourEmptyEvent);
	}

	for (const auto& Mag : MagazinesManager->Magazines)
	{
		if (!IsValid(Mag))
			continue;

		Mag->OnMagazineRefilled.RemoveDynamic(this, &UShooter::CallMagazineRefilledEvent);
		Mag->OnMagazineAmmoChanged.RemoveDynamic(this, &UShooter::CallMagazineAmmoChangedEvent);
		Mag->OnMagazineFull.RemoveDynamic(this, &UShooter::CallMagazineFullEvent);
		Mag->OnMagazineEmpty.RemoveDynamic(this, &UShooter::CallMagazineEmptyEvent);
	}
}

void UShooter::TickBehaviours(const float DeltaTime) const
{
	for (const auto Behaviour : ShooterBehaviours)
	{
		if (!IsValid(Behaviour))
			continue;

		Behaviour->TickBehaviour(DeltaTime);
	}
}

void UShooter::CallBehaviourEnabledEvent(UShooterBehaviourBase* Behaviour)
{
	OnBehaviourEnabled.Broadcast(Behaviour);
}

void UShooter::CallBehaviourDisabledEvent(UShooterBehaviourBase* Behaviour)
{
	OnBehaviourDisabled.Broadcast(Behaviour);
}

void UShooter::CallBehaviourShootSuccessEvent(UShooterBehaviourBase* Behaviour, UShootBarrel* ShootBarrel, int32 ShotIndex)
{
	OnBehaviourShootSuccess.Broadcast(Behaviour, ShootBarrel, ShotIndex);
}

void UShooter::CallBehaviourShootFailEvent(UShooterBehaviourBase* Behaviour, EShootFailReason FailReason)
{
	OnBehaviourShootFail.Broadcast(Behaviour, FailReason);
}

void UShooter::CallBehaviourShootSequenceEndedEvent(UShooterBehaviourBase* Behaviour)
{
	OnBehaviourShootSequenceEnded.Broadcast(Behaviour);
}

void UShooter::CallBehaviourAmmoChangedEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize)
{
	OnBehaviourAmmoChanged.Broadcast(Behaviour, Magazine, CurrentAmmo, MagSize);
}

void UShooter::CallBehaviourRefilledEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
	OnBehaviourRefilled.Broadcast(Behaviour, Magazine, CurrentAmmo, RefilledAmmo, RemainingAmmo);
}

void UShooter::CallBehaviourFullEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine)
{
	OnBehaviourFull.Broadcast(Behaviour, Magazine);
}

void UShooter::CallBehaviourEmptyEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine)
{
	OnBehaviourEmpty.Broadcast(Behaviour, Magazine);
}

void UShooter::CallMagazineRefilledEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
	OnMagazineRefilled.Broadcast(Instigator, Magazine, CurrentAmmo, RefilledAmmo, RemainingAmmo);
}

void UShooter::CallMagazineAmmoChangedEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize)
{
	OnMagazineAmmoChanged.Broadcast(Instigator, Magazine, CurrentAmmo, MagSize);
}

void UShooter::CallMagazineFullEvent(const UObject* Instigator, const UMagazine* Magazine)
{
	OnMagazineFull.Broadcast(Instigator, Magazine);
}

void UShooter::CallMagazineEmptyEvent(const UObject* Instigator, const UMagazine* Magazine)
{
	OnMagazineEmpty.Broadcast(Instigator, Magazine);
}
