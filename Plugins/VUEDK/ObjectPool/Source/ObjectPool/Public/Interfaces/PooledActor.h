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
	UFUNCTION(BlueprintNativeEvent)
	void OnPooledActorBeginPlay();

	UFUNCTION(BlueprintNativeEvent)
	void OnPooledActorEndPlay();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AssignActorPool(UActorPool* InActorPool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UActorPool* GetActorPool() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReleasePooledActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearPooledActor();
};
