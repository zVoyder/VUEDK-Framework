// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolSettings.h"
#include "PoolsManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogObjectPool, All, All);

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
	const FPoolsManagersData* FindPoolsManagersDataForGamemode(const AGameModeBase* GameMode) const;
	
	bool Check() const;
};
