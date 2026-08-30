// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolSettings.h"
#include "PoolsManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogObjectPool, All, All);

UCLASS(NotBlueprintable, BlueprintType)
class OBJECTPOOL_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	const UObjectPoolSettings* ObjectPoolSettings;
	UPROPERTY()
	UPoolsManager* CurrentPoolsManager;

public:
	UObjectPoolSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	/**
	 * Finds the pools manager for the given game mode.
	 * @param GameMode The game mode to find the pools manager for.
	 * @return A pointer to the pools manager data for the game mode, or nullptr if not found.
	 */
	const FPoolsManagersData* FindPoolsManagersDataForGamemode(const AGameModeBase* GameMode) const;

	/**
	 * Checks if the object pool subsystem is properly initialized.
	 * @return True if the subsystem is initialized, false otherwise.
	 */
	bool Check() const;
};
