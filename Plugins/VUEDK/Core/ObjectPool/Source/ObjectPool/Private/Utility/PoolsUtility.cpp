// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/PoolsUtility.h"
#include "ObjectPoolSubsystem.h"

TWeakObjectPtr<UPoolsManager> UPoolsUtility::PoolsManager = nullptr;

void UPoolsUtility::Init(UPoolsManager* InPoolsManager)
{
	PoolsManager = InPoolsManager;
}

UActorPool* UPoolsUtility::GetPoolByTag(const FGameplayTag PoolTag)
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Warning, TEXT("UPoolsUtility::GetPool: Invalid pools manager."));
		return nullptr;
	}

	return PoolsManager.Get()->GetPoolByTag(PoolTag);
}

UActorPool* UPoolsUtility::GetPoolByClass(const TSubclassOf<AActor> ActorClass)
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Warning, TEXT("UPoolsUtility::GetPoolByClass: Invalid pools manager."));
		return nullptr;
	}

	return PoolsManager.Get()->GetPoolByClass(ActorClass);
}

bool UPoolsUtility::Check()
{
	return PoolsManager.IsValid();
}
