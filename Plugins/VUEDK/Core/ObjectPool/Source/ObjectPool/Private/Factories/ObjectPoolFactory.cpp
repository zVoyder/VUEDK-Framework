// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/ObjectPoolFactory.h"
#include "ObjectPoolSubsystem.h"

UActorPool* UObjectPoolFactory::CreatePool(UWorld* WorldContextObject, const FActorPoolData& PoolData)
{
	UActorPool* Pool = NewObject<UActorPool>();

	if (!IsValid(Pool))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ObjectPoolFactory::CreatePool: Creation failed."));
		return nullptr;
	}
	
	Pool->Init(WorldContextObject, PoolData);
	return Pool;
}
