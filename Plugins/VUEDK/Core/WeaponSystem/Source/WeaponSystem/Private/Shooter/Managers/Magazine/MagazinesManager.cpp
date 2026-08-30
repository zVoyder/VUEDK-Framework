// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Managers/Magazine/MagazinesManager.h"
#include "Shooter/Shooter.h"

void UMagazinesManager::Init(UShooter* InShooter)
{
	Shooter = InShooter;
	for (UMagazine* Mag : Magazines)
		Mag->Init(this);
}

UWorld* UMagazinesManager::GetWorld() const
{
	return IsValid(Shooter) ? Shooter->GetWorld() : Super::GetWorld();
}

UMagazine* UMagazinesManager::GetMagazine(const int32 Index) const
{
	return HasMagazineOfIndex(Index) ? Magazines[Index] : nullptr;
}

bool UMagazinesManager::HasMagazineOfIndex(const int32 Index) const
{
	return Magazines.IsValidIndex(Index);
}
