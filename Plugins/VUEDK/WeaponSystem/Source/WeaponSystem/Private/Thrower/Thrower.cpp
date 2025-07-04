// Copyright VUEDK, Inc. All Rights Reserved.

#include "WeaponSystem/Public/Thrower/Thrower.h"
#include "WeaponSystem.h"
#include "Utility/PoolsUtility.h"

UThrower::UThrower()
{
	PrimaryComponentTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(this);
	ArrowComponent->ArrowSize = 0.2f;
#endif
}

void UThrower::Throw(const float Force, const float AngularSpeed) const
{
	ThrowAtDirection(Force, AngularSpeed, GetComponentRotation().Vector());
}

void UThrower::ThrowAtDirection(const float Force, const float AngularSpeed, const FVector& Direction) const
{
	AThrowableBase* Throwable = GetThrowable();

	if (!IsValid(Throwable))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UThrower::ThrowAtDirection: Throwable is not valid."));
		return;
	}

	Throwable->SetActorLocationAndRotation(GetComponentLocation(), Direction.Rotation());
	Throwable->Init(GetOwner());
	Throwable->GetMeshComponent()->AddImpulse(Direction * Force, NAME_None, true);
	Throwable->GetMeshComponent()->AddAngularImpulseInDegrees(Throwable->GetSpinAxis() * AngularSpeed, NAME_None, true);
}

AThrowableBase* UThrower::GetThrowable() const
{
	UActorPool* Pool = UPoolsUtility::GetPool(ThrowableTag);

	if (!IsValid(Pool))
		return nullptr;

	return Cast<AThrowableBase>(Pool->AcquireActor());
}
