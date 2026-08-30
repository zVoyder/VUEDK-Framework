// Copyright VUEDK, Inc. All Rights Reserved.

#include "Drop/DropItemOperation.h"
#include "InventorySystem.h"
#include "Base/InventoryBase.h"

bool UDropItemOperation::DropItem_Implementation(UItemBase* Item)
{
	AItemDropActor* DropActor = nullptr;
	return TryDropItemAroundPlayer(Item, DropActor); // Default drop around player
}

void UDropItemOperation::Init(APlayerController* InInstigator)
{
	Instigator = InInstigator;
}

APlayerController* UDropItemOperation::GetInstigator() const
{
	return Instigator;
}

UWorld* UDropItemOperation::GetWorld() const
{
	return IsValid(Instigator) ? Instigator->GetWorld() : nullptr;
}

#if WITH_EDITOR
bool UDropItemOperation::ImplementsGetWorld() const
{
	return true;
}
#endif

bool UDropItemOperation::TryDropItemAroundActor(UItemBase* Item, AActor* Actor, AItemDropActor*& OutItemDropActor) const
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UDropItemOperation::TryDropItemAroundActor: Item is invalid."));
		return false;
	}

	if (!IsValid(Actor))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UDropItemOperation::TryDropItemAroundActor: Actor is invalid."));
		return false;
	}
	
	const FVector Location = Actor->GetActorLocation();
	FVector AddVector = FVector(FMath::FRandRange(MinRangeDrop, MaxRangeDrop), FMath::FRandRange(MinRangeDrop, MaxRangeDrop), DropHeight);
	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Yaw = FMath::FRandRange(0.f, 360.0f);
	AddVector = Rotator.RotateVector(AddVector);
	
	const FVector DropLocation = Location + AddVector;
	FRotator DropRotation = FRotator::ZeroRotator;
	DropRotation.Yaw = FMath::FRandRange(0.f, 360.0f);
	return Item->TryDrop(Instigator, DropLocation, DropRotation, OutItemDropActor);
}

bool UDropItemOperation::TryDropItemAroundPlayer(UItemBase* Item, AItemDropActor*& OutItemDropActor) const
{
	if (!Check())
		return false;
	
	APawn* Pawn = Instigator->GetPawnOrSpectator();
	return TryDropItemAroundActor(Item, Pawn, OutItemDropActor);
}

bool UDropItemOperation::Check() const
{
	return IsValid(Instigator);
}
