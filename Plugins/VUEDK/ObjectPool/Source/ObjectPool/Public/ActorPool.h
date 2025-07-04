// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ActorPoolData.h"
#include "UObject/Object.h"
#include "ActorPool.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class OBJECTPOOL_API UActorPool : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> AvailableActors;
	UPROPERTY()
	TArray<AActor*> UsedActors;
	UPROPERTY()
	TSubclassOf<AActor> PooledActorClass;
	UPROPERTY()
	TObjectPtr<UWorld> WorldContextObject;
	FActorPoolData PoolData;

public:
	/**
	 * Initializes the actor pool with the given world context and pool data.
	 * @param InWorldContextObject The world context object.
	 * @param InPoolData The data describing the pool configuration.
	 */
	void Init(const TObjectPtr<UWorld>& InWorldContextObject, const FActorPoolData InPoolData);

	/**
	 * Destroys all actors in the pool and cleans up resources.
	 */
	UFUNCTION(BlueprintCallable)
	void DestroyPool();

	/**
	 * Returns the class of actors managed by this pool.
	 * @return The actor class.
	 */
	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetActorClass() const;

	/**
	 * Acquires an actor from the pool.
	 * @return Pointer to the acquired AActor, or nullptr if none available.
	 */
	UFUNCTION(BlueprintPure)
	AActor* AcquireActor();

	/**
	 * Releases an actor back to the pool.
	 * @param InActor The actor to release.
	 */
	UFUNCTION(BlueprintCallable)
	void ReleaseActor(AActor* InActor);

	/**
	 * Sets the actor class for this pool.
	 * @param InActorClass The actor class to set.
	 */
	void SetActorClass(const TSubclassOf<AActor>& InActorClass);

	/**
	 * Returns the world associated with this pool.
	 * @return Pointer to the UWorld.
	 */
	virtual UWorld* GetWorld() const override;

private:
	/**
	 * Spawns the specified number of actor instances for the pool.
	 * @param InCount The number of instances to spawn.
	 */
	void SpawnInstances(const int32 InCount);

	/**
	 * Spawns a single actor instance and adds it to the pool.
	 */
	void SpawnInstance();

	/**
	 * Pushes an actor onto the available pool stack.
	 * @param Actor The actor to push.
	 */
	void PushActor(AActor* Actor);

	/**
	 * Pops an actor from the available pool stack.
	 * @return Pointer to the popped AActor, or nullptr if pool is empty.
	 */
	AActor* PopActor();

	/**
	 * Checks if the pool is full.
	 * @return True if full, false otherwise.
	 */
	bool IsFull() const;

	/**
	 * Checks if the pool is empty.
	 * @return True if empty, false otherwise.
	 */
	bool IsEmpty() const;

	/**
	 * Checks the validity or state of the actor pool.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;

	/**
	 * Enables or disables an actor.
	 * @param Actor The actor to modify.
	 * @param bIsEnabled Whether to enable or disable the actor.
	 */
	void SetActorEnabled(AActor* Actor, bool bIsEnabled) const;

	/**
	 * Clears and resets an actor.
	 * @param Actor The actor to clear.
	 */
	static void ClearActor(AActor* Actor);
};
