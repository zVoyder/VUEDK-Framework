// Copyright VUEDK, Inc. All Rights Reserved.

#include "ActorPool.h"
#include "ObjectPoolSubsystem.h"
#include "Interfaces/PooledActor.h"

void UActorPool::Init(const TObjectPtr<UWorld>& InWorldContextObject, const FActorPoolData& InPoolData)
{
	if (!IsValid(InWorldContextObject))
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::Init: Invalid owner."));
		return;
	}

	WorldContextObject = InWorldContextObject;
	PoolData = InPoolData;
	PooledActorClass = PoolData.ActorClass.LoadSynchronous();
	if (!IsValid(PooledActorClass) || !PooledActorClass->ImplementsInterface(UPooledActor::StaticClass()))
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::Init: Invalid actor class for pool %s."), *PoolData.PoolTag.ToString());
		return;
	}
	
	if (PoolData.InitialSize < 0 )
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::Init: Invalid pool size for pool %s."), *PoolData.PoolTag.ToString());
		return;
	}

	if (PoolData.bIsCapped)
	{
		if (PoolData.MaxSize <= 0)
		{
			UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::Init: Invalid max size for pool %s."), *PoolData.PoolTag.ToString());
			return;
		}
		
		if (PoolData.InitialSize > PoolData.MaxSize)
		{
			UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::Init: Invalid max size for pool %s. Initial size cannot be greater than max size."), *PoolData.PoolTag.ToString());
			return;
		}
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

FGameplayTag UActorPool::GetPoolTag() const
{
	return PoolData.PoolTag;
}

TSubclassOf<AActor> UActorPool::GetActorClass() const
{
	return PooledActorClass;
}

AActor* UActorPool::AcquireActorAtLocation(const FVector Location)
{
	FTransform Transform;
	Transform.SetLocation(Location);
	return AcquireActor(Transform);
}

AActor* UActorPool::AcquireActorAtLocationAndRotation(const FVector Location, const FRotator Rotation)
{
	FTransform Transform;
	Transform.SetLocation(Location);
	Transform.SetRotation(Rotation.Quaternion());
	return AcquireActor(Transform);
}

AActor* UActorPool::AcquireActor(const FTransform& SpawnTransform, const bool bApplyScale)
{
	if (IsEmpty())
	{
		if (PoolData.bIsCapped)
		{
			UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::AcquireActor: Cannot acquire actor, pool is empty and capped."));
			return nullptr;
		}

		SpawnInstance(SpawnTransform, bApplyScale);
	}

	return PopActor(SpawnTransform, bApplyScale);
}

AActor* UActorPool::ReceiveAcquireActor(const FTransform SpawnTransform, const bool bApplyScale)
{
	return AcquireActor(SpawnTransform, bApplyScale);
}

void UActorPool::ReleaseActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::ReleaseActor: Invalid actor."));
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
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::SetActorClass: Invalid actor class."));
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
	const int32 InstancesCount = FMath::Clamp(InCount, 0, PoolData.MaxSize - AvailableActors.Num());

	for (int32 i = 0; i < InstancesCount; i++)
		SpawnInstance();
}

void UActorPool::SpawnInstance(const FTransform& SpawnTransform, const bool bApplyScale)
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

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bNoFail = true;
	AActor* Actor =
		bApplyScale ?
		World->SpawnActor<AActor>(PooledActorClass, SpawnTransform, Params) :
		World->SpawnActor<AActor>(PooledActorClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator(), Params);
	
	if (!IsValid(Actor))
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::SpawnInstance: Failed to spawn actor of class %s."), *PooledActorClass->GetFullName());
		return;
	}

	IPooledActor::Execute_AssignActorPool(Actor, this);
	PushActor(Actor);
	Actor->PrimaryActorTick.bStartWithTickEnabled = false;
}

void UActorPool::PushActor(AActor* Actor)
{
	SetActorEnabled(Actor, false);
	ClearActor(Actor);
	AvailableActors.Push(Actor);
	UsedActors.Remove(Actor);
}

AActor* UActorPool::PopActor(const FTransform& SpawnTransform, const bool bApplyScale)
{
	// It could happen that the array has invalid actors,
	// so we need to check until we find a valid one or the array is empty
	while (AvailableActors.Num() > 0)
	{
		AActor* Actor = AvailableActors.Pop();

		if (!IsValid(Actor))
		{
			UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::PopActor: Removed invalid actor."));
			continue;
		}

		if (bApplyScale)
			Actor->SetActorTransform(SpawnTransform);
		else
			Actor->SetActorLocationAndRotation(SpawnTransform.GetLocation(), SpawnTransform.GetRotation());

		SetActorEnabled(Actor, true);
		UsedActors.Push(Actor);
		return Actor;
	}

	UE_LOG(LogObjectPool, Warning, TEXT("ActorPool::PopActor: No valid actors available."));
	return nullptr;
}

bool UActorPool::IsFull() const
{
	if (!PoolData.bIsCapped)
		return false;
	
	return AvailableActors.Num() >= PoolData.MaxSize;
}

bool UActorPool::IsEmpty() const
{
	return AvailableActors.IsEmpty();
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

	USceneComponent* RootComponent = Actor->GetRootComponent();
	if (IsValid(RootComponent))
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent);
		if (IsValid(PrimitiveComponent))
		{
			if (PrimitiveComponent->IsSimulatingPhysics())
			{
				PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
				PrimitiveComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
			}
		}
	}

	Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	IPooledActor::Execute_ClearPooledActor(Actor);
}
