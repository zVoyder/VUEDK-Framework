// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectPoolFactory.generated.h"

UCLASS()
class OBJECTPOOL_API UObjectPoolFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates an actor pool based on the provided pool data.
	 * @param WorldContextObject The world context object to use for the pool.
	 * @param PoolData The data defining the pool configuration, including the actor class and initial size.
	 * @return A pointer to the created UActorPool instance, or nullptr if creation failed.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "ObjectPool")
	static UActorPool* CreatePool(UWorld* WorldContextObject, const FActorPoolData& PoolData);
};
