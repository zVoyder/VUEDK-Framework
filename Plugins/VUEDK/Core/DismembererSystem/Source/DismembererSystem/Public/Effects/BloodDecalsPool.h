// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BloodDecal.h"
#include "DismembererSystemSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "BloodDecalsPool.generated.h"

UCLASS()
class DISMEMBERERSYSTEM_API UBloodDecalsPool : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<TObjectPtr<ABloodDecal>> AvailableDecals;
	UPROPERTY()
	TArray<TObjectPtr<ABloodDecal>> SpawnedDecals;
	UPROPERTY()
	const UDismembererSystemSettings* DismembererSystemSettings;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	/**
	 * Destroys all decals in the pool and cleans up resources.
	 */
	void DestroyPool();
	
	/**
	 * Acquires a blood decal from the pool.
	 * @return Pointer to the acquired ABloodDecal, or nullptr if none available.
	 */
	ABloodDecal* AcquireDecal();
	
	/**
	 * Releases a blood decal back to the pool.
	 * @param InActor The decal actor to release.
	 */
	void ReleaseDecal(ABloodDecal* InActor);

private:
	/**
	 * Checks if the current world game mode is allowed to use decals.
	 * @param InWorld The world context to check.
	 * @return True if decals are allowed, false otherwise.
	 */
	bool AreDecalsAllowedForWorld(const UWorld& InWorld) const;
	
	/**
	 * Spawns all decal instances for the pool.
	 */
	void SpawnInstances();

	/**
	 * Spawns a single decal instance and adds it to the pool.
	 */
	void SpawnInstance();

	/**
	 * Pushes a decal actor onto the available pool stack.
	 * @param Actor The decal actor to push.
	 */
	void PushDecal(ABloodDecal* Actor);

	/**
	 * Pops a decal actor from the available pool stack.
	 * @return Pointer to the popped ABloodDecal, or nullptr if pool is empty.
	 */
	ABloodDecal* PopDecal();

	/**
	 * Checks if the pool of available decals is empty.
	 * @return True if empty, false otherwise.
	 */
	bool IsEmpty() const;

	/**
	 * Enables or disables a decal actor.
	 * @param Actor The decal actor to modify.
	 * @param bIsEnabled Whether to enable or disable the actor.
	 */
	static void SetDecalEnabled(ABloodDecal* Actor, bool bIsEnabled);

	/**
	 * Clears and resets a decal actor.
	 * @param Actor The decal actor to clear.
	 */
	static void ClearDecal(ABloodDecal* Actor);
};
