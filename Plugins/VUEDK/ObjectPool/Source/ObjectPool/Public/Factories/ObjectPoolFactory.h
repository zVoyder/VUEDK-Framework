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
	UFUNCTION(BlueprintCallable, Category = "Object Pool Factory", meta = (WorldContext = "WorldContextObject"))
	static UActorPool* CreatePool(UWorld* WorldContextObject, const FActorPoolData& PoolData);
};
