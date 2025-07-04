// Copyright VUEDK, Inc. All Rights Reserved.

#include "PooledActorBase.h"
#include "ActorPool.h"
#include "ObjectPoolSubsystem.h"

APooledActorBase::APooledActorBase(): ActorPool(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APooledActorBase::NativeReleasePooledActor()
{
	IPooledActor::Execute_ReleasePooledActor(this);
}

UActorPool* APooledActorBase::NativeGetActorPool() const
{
	return IPooledActor::Execute_GetActorPool(this);
}

void APooledActorBase::AssignActorPool_Implementation(UActorPool* InActorPool)
{
	ActorPool = InActorPool;
}

UActorPool* APooledActorBase::GetActorPool_Implementation() const
{
	return ActorPool;
}

void APooledActorBase::ReleasePooledActor_Implementation()
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Warning, TEXT("APooledActorBase::ReleasePooledActor(): Invalid actor pool, destroying actor."));
		Destroy();
		return;
	}

	ActorPool->ReleaseActor(this);
}

void APooledActorBase::ClearPooledActor_Implementation()
{
}

bool APooledActorBase::Check() const
{
	return IsValid(ActorPool);
}
