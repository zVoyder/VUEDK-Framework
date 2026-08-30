// Copyright VUEDK, Inc. All Rights Reserved.

#include "PoolsManager.h"
#include "ObjectPoolSubsystem.h"
#include "Factories/ObjectPoolFactory.h"
#include "Utility/PoolsUtility.h"

UPoolsManager::UPoolsManager() : WorldContextObject(nullptr)
{
}

void UPoolsManager::Init(UWorld* InWorldContextObject, const TArray<FActorPoolData>& InPoolsData)
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

UActorPool* UPoolsManager::GetPoolByTag(const FGameplayTag PoolTag) const
{
	if (!PoolTag.IsValid())
	{
		UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::GetPoolByTag: PoolTag is not valid."));
		return nullptr;
	}
	
	for (UActorPool* Pool : ActorPools)
	{
		if (Pool->GetPoolTag() == PoolTag)
			return Pool;
	}

	UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::GetPoolByTag: Pool with tag %s not found."), *PoolTag.ToString());
	return nullptr;
}

UActorPool* UPoolsManager::GetPoolByClass(const TSubclassOf<AActor> ActorClass) const
{
	if (!IsValid(ActorClass))
	{
		UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::GetPoolByClass: ActorClass is null."));
		return nullptr;
	}

	for (UActorPool* Pool : ActorPools)
	{
		TSubclassOf<AActor> PoolActorClass = Pool->GetActorClass();
		if (!IsValid(PoolActorClass))
			continue;

		if (PoolActorClass->GetClassPathName() == ActorClass->GetClassPathName())
			return Pool;
	}

	UE_LOG(LogObjectPool, Error, TEXT("UPoolsManager::GetPoolByClass: Pool of class %s not found."), *ActorClass->GetClassPathName().ToString());
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
		if (!IsValidPool(PoolData))
			continue;

		UActorPool* ActorPool = UObjectPoolFactory::CreatePool(WorldContextObject, PoolData);
		if (!IsValid(ActorPool))
			continue;

		ActorPools.Add(ActorPool);
	}
}

void UPoolsManager::DestroyPools()
{
	for (UActorPool* Pool : ActorPools)
	{
		if (IsValid(Pool))
			Pool->DestroyPool();
	}
}

bool UPoolsManager::IsValidPool(const FActorPoolData& PoolData) const
{
	if (PoolData.ActorClass.IsNull())
	{
		UE_LOG(LogObjectPool, Warning, TEXT("UPoolsManager::IsValidPool: PoolData contains a null ActorClass reference."));
		return false;
	}
	
	for (const UActorPool* Pool : ActorPools)
	{
		FGameplayTag Tag = Pool->GetPoolTag();
		if (Tag.IsValid())
		{
			if (Pool->GetPoolTag() == PoolData.PoolTag)
			{
				UE_LOG(LogObjectPool, Warning, TEXT("UPoolsManager::IsValidPool: A pool with tag %s is already registered. Duplicate pool tags are not allowed."), *PoolData.PoolTag.ToString());
				return false;
			}
		}
	}

	return true;
}

bool UPoolsManager::Check() const
{
	return IsValid(WorldContextObject);
}
