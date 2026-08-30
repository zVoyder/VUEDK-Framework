// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Managers/Magazine/Magazine.h"
#include "Shooter/Managers/Magazine/MagazinesManager.h"

void UMagazine::Init(UMagazinesManager* InMagazinesManager)
{
	MagazinesManager = InMagazinesManager;
	CurrentSize = MagazineData.DefaultSize;

	if (bInitiallyFull)
		CurrentAmmoCount = CurrentSize;
	else
		CurrentAmmoCount = 0;
}

UWorld* UMagazine::GetWorld() const
{
	return IsValid(MagazinesManager) ? MagazinesManager->GetWorld() : Super::GetWorld();
}

void UMagazine::Tick(float DeltaTime)
{
	if (bEmptyPending)
	{
		CallMagEmptyEvent(PendingInstigator.Get());
		bEmptyPending = false;
	}

	if (bFullPending)
	{
		CallMagFullEvent(PendingInstigator.Get());
		bFullPending = false;
	}
}

bool UMagazine::IsTickable() const
{
	return bEmptyPending || bFullPending;
}

TStatId UMagazine::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMagazine, STATGROUP_Tickables);
}

void UMagazine::SetCurrentAmmo(const int32 NewAmmo, UObject* Instigator)
{
	const int32 MagSize = GetMagazineSize();
	const int32 OldAmmo = CurrentAmmoCount;
	CurrentAmmoCount = FMath::Clamp(NewAmmo, 0, MagSize);
	CallAmmoChangeEvent(MagSize, Instigator);

	const bool bBecameEmpty = (OldAmmo > 0 && CurrentAmmoCount == 0);
	const bool bBecameFull = (OldAmmo < MagSize && CurrentAmmoCount == MagSize);
	bEmptyPending = bBecameEmpty;
	bFullPending = bBecameFull;
	PendingInstigator = Instigator;
}

void UMagazine::ModifyCurrentAmmo(const int32 Ammo, UObject* Instigator)
{
	SetCurrentAmmo(CurrentAmmoCount + Ammo, Instigator);
}

bool UMagazine::TryConsumeAmmo(const int32 Ammo, UObject* Instigator)
{
	if (!CanConsumeAmmo(Ammo))
		return false;

	ModifyCurrentAmmo(-Ammo, Instigator);
	return true;
}

int32 UMagazine::Refill(const int32 Ammo, int32& OutRemainingAmmo, UObject* Instigator)
{
	const int32 OldAmmo = CurrentAmmoCount;
	const int32 SpaceLeft = GetMagazineSize() - OldAmmo;

	const int32 RefilledAmmo = FMath::Min(SpaceLeft, Ammo);
	OutRemainingAmmo = Ammo - RefilledAmmo;
	ModifyCurrentAmmo(RefilledAmmo, Instigator);
	CallRefillEvent(OutRemainingAmmo, RefilledAmmo, Instigator);
	return RefilledAmmo;
}

void UMagazine::RefillAllMagazine(UObject* Instigator)
{
	int32 RemainingAmmo = 0;
	Refill(GetNeededAmmoToFull(), RemainingAmmo, Instigator);
}

int32 UMagazine::RefillWithAmmoType(UAmmoTypeData* AmmoType, const int32 Ammo, int32& OutRemainingAmmo, UObject* Instigator)
{
	if (!IsOfAmmoType(AmmoType))
	{
		OutRemainingAmmo = Ammo;
		return 0;
	}

	return Refill(Ammo, OutRemainingAmmo, Instigator);
}

void UMagazine::SetMagazineSize(int32 NewSize)
{
	NewSize = NewSize < 0 ? 0 : NewSize;
	CurrentSize = NewSize;

	if (CurrentAmmoCount > CurrentSize)
		CurrentAmmoCount = CurrentSize;
}

int32 UMagazine::GetMagazineSize() const
{
	return CurrentSize;
}

UAmmoTypeData* UMagazine::GetAmmoType() const
{
	return MagazineData.AmmoType;
}

int32 UMagazine::GetCurrentAmmo() const
{
	return CurrentAmmoCount;
}

int32 UMagazine::GetNeededAmmoToFull() const
{
	return CurrentSize - CurrentAmmoCount;
}

bool UMagazine::IsFull() const
{
	return CurrentAmmoCount >= CurrentSize;
}

bool UMagazine::IsEmpty() const
{
	return CurrentAmmoCount <= 0;
}

bool UMagazine::IsOfAmmoType(const UAmmoTypeData* AmmoType) const
{
	return AmmoType == MagazineData.AmmoType;
}

bool UMagazine::CanConsumeAmmo(const int32 Ammo) const
{
	return CurrentAmmoCount >= Ammo;
}

void UMagazine::ResetMagazineSizeToDefault()
{
	SetMagazineSize(MagazineData.DefaultSize);
}

void UMagazine::OnRefill_Implementation(const UObject* Instigator, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo)
{
}

void UMagazine::OnAmmoChange_Implementation(const UObject* Instigator, int32 CurrentAmmo, int32 MagSize)
{
}

void UMagazine::OnFull_Implementation(UObject* Instigator)
{
}

void UMagazine::OnEmpty_Implementation(UObject* Instigator)
{
}

void UMagazine::CallAmmoChangeEvent(const int32 MagSize, const UObject* Instigator)
{
	OnAmmoChange(Instigator, CurrentAmmoCount, MagSize);
	OnMagazineAmmoChanged.Broadcast(Instigator, this, CurrentAmmoCount, MagSize);
}

void UMagazine::CallRefillEvent(const int32& OutRemainingAmmo, const int32 RefilledAmmo, const UObject* Instigator)
{
	OnRefill(Instigator, CurrentAmmoCount, RefilledAmmo, OutRemainingAmmo);
	OnMagazineRefilled.Broadcast(Instigator, this, CurrentAmmoCount, RefilledAmmo, OutRemainingAmmo);
}

void UMagazine::CallMagEmptyEvent(UObject* Instigator)
{
	OnEmpty(Instigator);
	OnMagazineEmpty.Broadcast(Instigator, this);
}

void UMagazine::CallMagFullEvent(UObject* Instigator)
{
	OnFull(Instigator);
	OnMagazineFull.Broadcast(Instigator, this);
}
