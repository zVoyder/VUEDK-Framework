// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/DropItemOperation.h"
#include "Base/InventoryBase.h"
#include "Kismet/GameplayStatics.h"

bool UDropItemOperation::DropItem_Implementation(UItemBase* Item)
{
	AItemDropActor* DropActor = nullptr;
	return TryDropItemAroundPlayer(Item, DropActor); // Default drop around player
}

UWorld* UDropItemOperation::GetWorld() const
{
	return GEngine ? GEngine->GetWorld() : nullptr;
}

#if WITH_EDITOR
bool UDropItemOperation::ImplementsGetWorld() const
{
	return true;
}
#endif

bool UDropItemOperation::TryDropItemAroundActor(UItemBase* Item, AActor* Actor, AItemDropActor*& OutItemDropActor) const
{
	if (!IsValid(Item) || !IsValid(Actor))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("TryDropItemAroundActor: Item or Actor is not valid!"));
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
	return Item->TryDrop(DropLocation, DropRotation, OutItemDropActor);
}

bool UDropItemOperation::TryDropItemAroundPlayer(UItemBase* Item, AItemDropActor*& OutItemDropActor) const
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(Item, 0);

	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("TryDropItemAroundPlayer: PlayerPawn is not valid!"));
		return false;
	}

	return TryDropItemAroundActor(Item, PlayerPawn, OutItemDropActor);
}
