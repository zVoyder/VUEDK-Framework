// Copyright VUEDK, Inc. All Rights Reserved.

#include "ActorPool.h"
#include "ObjectPoolSubsystem.h"
#include "Interfaces/PooledActor.h"

void UActorPool::Init(const TObjectPtr<UWorld>& InWorldContextObject, const FActorPoolData InPoolData)
{
	if (!IsValid(InWorldContextObject))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::Init: Invalid owner."));
		return;
	}

	WorldContextObject = InWorldContextObject;
	PoolData = InPoolData;
	PooledActorClass = PoolData.ActorClass.LoadSynchronous();
	if (!IsValid(PooledActorClass) || !PooledActorClass->ImplementsInterface(UPooledActor::StaticClass()))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::Init: Invalid actor class."));
		return;
	}
	
	if (PoolData.InitialSize < 0 || PoolData.MaxSize < 0 || PoolData.InitialSize > PoolData.MaxSize)
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::Init: Invalid pool size."));
		return;
	}

	AddToRoot();
	SpawnInstances(PoolData.InitialSize);
}

void UActorPool::DestroyPool()
{
	for (AActor* Actor : AvailableActors)
	{
		if (IsValid(Actor))
			Actor->Destroy();
	}

	PooledActorClass = nullptr;
	AvailableActors.Empty();
	RemoveFromRoot();
}

TSubclassOf<AActor> UActorPool::GetActorClass() const
{
	return PooledActorClass;
}

AActor* UActorPool::AcquireActor()
{
	if (IsEmpty())
	{
		if (PoolData.bIsCapped)
		{
			UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::AcquireActor: Cannot acquire actor, pool is empty and capped."));
			return nullptr;
		}

		SpawnInstance();
	}
	
	return PopActor();
}

void UActorPool::ReleaseActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::ReleaseActor: Invalid actor."));
		return;
	}

	if (AvailableActors.Contains(InActor))
		return;
	
	if (IsFull() && PoolData.bIsCapped)
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::ReleaseActor: Cannot release actor, pool is full and capped."));
		InActor->Destroy();
		return;
	}
	
	PushActor(InActor);
}

void UActorPool::SetActorClass(const TSubclassOf<AActor>& InActorClass)
{
	if (!IsValid(InActorClass) || !InActorClass->ImplementsInterface(UPooledActor::StaticClass()))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::SetActorClass: Invalid actor class."));
		return;
	}

	PooledActorClass = InActorClass;
}

UWorld* UActorPool::GetWorld() const
{
	return WorldContextObject;
}

void UActorPool::SpawnInstances(const int32 InCount)
{
	int32 InstancesCount = FMath::Clamp(InCount, 0, PoolData.MaxSize - AvailableActors.Num());
	
	for (int32 i = 0; i < InCount; i++)
		SpawnInstance();
}

void UActorPool::SpawnInstance()
{
	if (GEngine == nullptr)
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::SpawnInstance: GEngine is null."));
		return;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!IsValid(World))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::SpawnInstance: Invalid world (null after cast)."));
		return;
	}

	if (!IsValid(PooledActorClass))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::SpawnInstance: Invalid actor class %s."), *PooledActorClass->GetFullName());
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bNoFail = true;
	AActor* Actor = World->SpawnActor<AActor>(PooledActorClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	Actor->PrimaryActorTick.bStartWithTickEnabled = false;
	if (!IsValid(Actor))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::SpawnInstance: Failed to spawn actor of class %s."), *PooledActorClass->GetFullName());
		return;
	}

	IPooledActor::Execute_AssignActorPool(Actor, this);
	PushActor(Actor);
}

void UActorPool::PushActor(AActor* Actor)
{
	ClearActor(Actor);
	SetActorEnabled(Actor, false);
	AvailableActors.Push(Actor);
	UsedActors.Remove(Actor);
}

AActor* UActorPool::PopActor()
{
	const int LastIndex = AvailableActors.Num() - 1;
	if (!AvailableActors.IsValidIndex(LastIndex))
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::PopActor: Invalid index."));
		return nullptr;
	}
	
	AActor* Actor = AvailableActors[LastIndex];
	if (!IsValid(Actor))
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::PopActor: Invalid actor."));
		return nullptr;
	}
	
	SetActorEnabled(Actor, true);
	UsedActors.Push(Actor);
	AvailableActors.Remove(Actor);
	return Actor;
}

bool UActorPool::IsFull() const
{
	return AvailableActors.Num() >= PoolData.MaxSize;
}

bool UActorPool::IsEmpty() const
{
	return AvailableActors.Num() == 0;
}

bool UActorPool::Check() const
{
	return IsValid(PooledActorClass);
}

void UActorPool::SetActorEnabled(AActor* Actor, const bool bIsEnabled) const
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::SetActorEnabled(): Invalid actor."));
		return;
	}

	bIsEnabled ? Actor->RegisterAllComponents() : Actor->UnregisterAllComponents();
	Actor->SetActorHiddenInGame(!bIsEnabled);
	Actor->SetActorEnableCollision(bIsEnabled);
	Actor->SetActorTickEnabled(bIsEnabled);
	
	if (bIsEnabled)
		IPooledActor::Execute_OnPooledActorBeginPlay(Actor);
	else
		IPooledActor::Execute_OnPooledActorEndPlay(Actor);
	
#if WITH_EDITOR
	const FString ActorEnabled = PooledActorClass->GetName().Append(bIsEnabled ? TEXT("_InUse") : TEXT("_Available"));
	Actor->SetActorLabel(ActorEnabled, false);
	const FString FolderPathString = "Pools/Pool_" + PooledActorClass->GetName();
	const FName FolderPathName = FName(*FolderPathString);
	Actor->SetFolderPath(FolderPathName);
#endif
}

void UActorPool::ClearActor(AActor* Actor)
{
	if (!IsValid(Actor) || !Actor->Implements<UPooledActor>())
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool::ClearActor(): Invalid actor."));
		return;
	}

	Actor->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	IPooledActor::Execute_ClearPooledActor(Actor);
}
