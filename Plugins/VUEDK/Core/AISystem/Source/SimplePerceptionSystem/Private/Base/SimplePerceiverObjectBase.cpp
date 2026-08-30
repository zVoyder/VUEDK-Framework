// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/SimplePerceiverObjectBase.h"
#include "SimplePerceiver.h"

void USimplePerceiverObjectBase::Init(USimplePerceiver* InSimplePerceiver)
{
	SimplePerceiver = InSimplePerceiver;

	Owner = SimplePerceiver->GetOwner();
	if (!IsValid(Owner))
		return;

	// In case the owner is an AController
	if (Owner->IsA<AController>())
	{
		AController* OwnerController = Cast<AController>(Owner);
		Owner = OwnerController->GetPawn();
		OwnerController->GetOnNewPawnNotifier().AddLambda([this](APawn* NewPawn) // Wait for the pawn to be possessed
		{
			if (!IsValid(NewPawn))
				return;

			if (NewPawn == Owner)
				return;

			Owner = NewPawn;
			OnInit();
		});
	}

	OnInit();
}

USimplePerceiver* USimplePerceiverObjectBase::GetPerceiver() const
{
	return SimplePerceiver;
}

AActor* USimplePerceiverObjectBase::GetOwner() const
{
	return Owner;
}

FVector USimplePerceiverObjectBase::GetSightLocation() const
{
	if (!IsValid(SimplePerceiver) || !IsValid(Owner))
		return FVector::ZeroVector;

	return Owner->GetActorLocation() + SimplePerceiver->SightLocationOffset;
}

#if WITH_EDITOR
bool USimplePerceiverObjectBase::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* USimplePerceiverObjectBase::GetWorld() const
{
	return IsValid(SimplePerceiver) ? SimplePerceiver->GetWorld() : nullptr;
}
