// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/PoolsUtility.h"
#include "ObjectPoolSubsystem.h"

UPoolsManager* UPoolsUtility::PoolsManager = nullptr;

void UPoolsUtility::Init(UPoolsManager* InPoolsManager)
{
	PoolsManager = InPoolsManager;
}

UActorPool* UPoolsUtility::GetPool(const FGameplayTag PoolTag)
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Error, TEXT("UPoolsUtility::GetPool: Invalid pools manager."));
		return nullptr;
	}

	return PoolsManager->GetPool(PoolTag);
}

UActorPool* UPoolsUtility::GetPoolByClass(const UClass* ActorClass)
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Error, TEXT("UPoolsUtility::GetPoolByClass: Invalid pools manager."));
		return nullptr;
	}

	return PoolsManager->GetPoolByClass(ActorClass);
}

bool UPoolsUtility::Check()
{
	return IsValid(PoolsManager);
}
