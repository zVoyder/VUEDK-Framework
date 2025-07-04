// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PoolsManager.h"
#include "PoolsUtility.generated.h"

UCLASS()
class OBJECTPOOL_API UPoolsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UPoolsManager* PoolsManager;

public:
	static void Init(UPoolsManager* InPoolsManager);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Patterns|ObjectPool")
	static UActorPool* GetPool(FGameplayTag PoolTag);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Patterns|ObjectPool")
	static UActorPool* GetPoolByClass(const UClass* ActorClass);

private:
	static bool Check();
};
