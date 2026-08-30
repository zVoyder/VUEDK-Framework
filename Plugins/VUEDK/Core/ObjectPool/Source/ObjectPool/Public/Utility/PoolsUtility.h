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
	static TWeakObjectPtr<UPoolsManager> PoolsManager;

public:
	/**
	 * Initializes the PoolsUtility with the given PoolsManager.
	 * @param InPoolsManager The PoolsManager to use.
	 */
	static void Init(UPoolsManager* InPoolsManager);

	/**
	 * Returns the actor pool associated with the specified gameplay tag.
	 * @param PoolTag The gameplay tag identifying the pool.
	 * @return Pointer to the UActorPool if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	static UActorPool* GetPoolByTag(FGameplayTag PoolTag);

	/**
	 * Returns the first actor pool that manages actors of the specified class.
	 * Use GetPoolByTag if you need to get a specific pool.
	 * @param ActorClass The class of the actor to search for.
	 * @return Pointer to the UActorPool if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	static UActorPool* GetPoolByClass(const TSubclassOf<AActor> ActorClass);

private:
	/**
	 * Checks the validity or state of the PoolsUtility.
	 * @return True if valid, false otherwise.
	 */
	static bool Check();
};
