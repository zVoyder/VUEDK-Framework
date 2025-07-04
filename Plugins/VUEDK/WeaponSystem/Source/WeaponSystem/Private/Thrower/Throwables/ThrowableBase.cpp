// Copyright VUEDK, Inc. All Rights Reserved.

#include "Thrower/Throwables/ThrowableBase.h"
#include "WeaponSystem.h"

AThrowableBase::AThrowableBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowableMesh"));
	SetRootComponent(MeshComponent);
}

void AThrowableBase::Init(AActor* InInstigator)
{
	ThrowableInstigator = InInstigator;
}

UStaticMeshComponent* AThrowableBase::GetMeshComponent() const
{
	return MeshComponent;
}

FVector AThrowableBase::GetSpinAxis() const
{
	switch (ThrowableSpinAxis)
	{
	case ThrowableSpinAxis_Up:
		return GetActorUpVector();
	case ThrowableSpinAxis_Down:
		return -GetActorUpVector();
	case ThrowableSpinAxis_Left:
		return GetActorRightVector();
	case ThrowableSpinAxis_Right:
		return -GetActorRightVector();
	case ThrowableSpinAxis_Forward:
		return GetActorForwardVector();
	case ThrowableSpinAxis_Backward:
		return -GetActorForwardVector();
	case ThrowableSpinAxis_Custom:
		return GetCustomAxisSpin();
	default:
		return FVector::ZeroVector;
	}
}

FVector AThrowableBase::GetCustomAxisSpin_Implementation() const
{
	return FVector::ZeroVector;
}

void AThrowableBase::OnPooledActorBeginPlay_Implementation()
{
	Super::OnPooledActorBeginPlay_Implementation();
	MeshComponent->SetSimulatePhysics(true);
	SetLifeSpan(LifeTime);
}

void AThrowableBase::ClearPooledActor_Implementation()
{
	Super::ClearPooledActor_Implementation();
	MeshComponent->SetSimulatePhysics(false);
}

void AThrowableBase::InvalidateLifeSpanTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	
	if (LifeSpanHandle.IsValid())
		World->GetTimerManager().ClearTimer(LifeSpanHandle);
}

void AThrowableBase::OnThrowableLifeSpanEnd_Implementation()
{
}

void AThrowableBase::EndThrowableLifeSpan()
{
	OnThrowableLifeSpanEnd();
	NativeReleasePooledActor();
}

void AThrowableBase::SetLifeSpan(const float LifeSpan)
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("AThrowableBase::Init: World is not valid."));
		return;
	}

	LifeSpanHandle = FTimerHandle();
	World->GetTimerManager().SetTimer(LifeSpanHandle, this, &AThrowableBase::EndThrowableLifeSpan, LifeSpan, false);
}
