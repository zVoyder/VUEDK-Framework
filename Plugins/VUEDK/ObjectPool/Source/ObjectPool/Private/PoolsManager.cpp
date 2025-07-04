// Copyright VUEDK, Inc. All Rights Reserved.

#include "PoolsManager.h"
#include "ObjectPoolSubsystem.h"
#include "Factories/ObjectPoolFactory.h"
#include "Utility/PoolsUtility.h"

UPoolsManager::UPoolsManager(): WorldContextObject(nullptr)
{
}

void UPoolsManager::Init(UWorld* InWorldContextObject, const TArray<FActorPoolData> InPoolsData)
{
	if (!IsValid(InWorldContextObject))
	{
		UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::Init: WorldContextObject not valid."));
		return;
	}

	WorldContextObject = InWorldContextObject;
	PoolsData = InPoolsData;
	InitPools();
	UPoolsUtility::Init(this);
}

UActorPool* UPoolsManager::GetPool(const FGameplayTag PoolTag) const
{
	if (!ActorPools.Contains(PoolTag))
	{
		UE_LOG(LogObjectPool, Error, TEXT("Pool %s not found."), *PoolTag.ToString());
		return nullptr;
	}
	
	return ActorPools.FindRef(PoolTag);
}

UActorPool* UPoolsManager::GetPoolByClass(const UClass* ActorClass) const
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::GetPoolByClass: PoolsManager is not initialized."));
		return nullptr;
	}

	for (const auto& Pool : ActorPools)
	{
		if (IsValid(Pool.Value) && Pool.Value->GetActorClass() == ActorClass)
			return Pool.Value;
	}

	UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::GetPoolByClass: Pool for class %s not found."), *ActorClass->GetName());
	return nullptr;
}

void UPoolsManager::BeginDestroy()
{
	DestroyPools();
	UObject::BeginDestroy();
}

void UPoolsManager::InitPools()
{
	for (const auto& PoolData : PoolsData)
	{
		UActorPool* ActorPool = UObjectPoolFactory::CreatePool(WorldContextObject, PoolData);
		if (IsValid(ActorPool))
			ActorPools.Add(PoolData.PoolTag, ActorPool);
	}
}

void UPoolsManager::DestroyPools()
{
	for (const auto& Pool : ActorPools)
	{
		if (IsValid(Pool.Value))
			Pool.Value->DestroyPool();
	}
}

bool UPoolsManager::Check() const
{
	return IsValid(WorldContextObject);
}
