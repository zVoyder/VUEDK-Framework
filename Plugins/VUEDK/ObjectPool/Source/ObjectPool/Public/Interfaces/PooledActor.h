// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PooledActor.generated.h"

class UActorPool;

UINTERFACE(Blueprintable, BlueprintType)
class UPooledActor : public UInterface
{
	GENERATED_BODY()
};

class OBJECTPOOL_API IPooledActor
{
	GENERATED_BODY()

public:
	/**
	 * Called when the pooled actor begins play (spawned or reused from the pool).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPooledActorBeginPlay();

	/**
	 * Called when the pooled actor ends play (returned to the pool or destroyed).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPooledActorEndPlay();
	
	/**
	 * Assigns the actor pool to this pooled actor.
	 * @param InActorPool The actor pool to assign.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AssignActorPool(UActorPool* InActorPool);

	/**
	 * Returns the actor pool associated with this pooled actor.
	 * @return Pointer to the UActorPool.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UActorPool* GetActorPool() const;

	/**
	 * Releases this actor back to its pool.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReleasePooledActor();

	/**
	 * Clears the pool reference from this actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearPooledActor();
};
