// Copyright VUEDK, Inc. All Rights Reserved.

#include "ObjectPoolSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/PoolsUtility.h"

DEFINE_LOG_CATEGORY(LogObjectPool);

UObjectPoolSubsystem::UObjectPoolSubsystem(): ObjectPoolSettings(nullptr),
                                              CurrentPoolsManager(nullptr)
{
}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ObjectPoolSettings = GetDefault<UObjectPoolSettings>();
}

void UObjectPoolSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (IsValid(CurrentPoolsManager))
		CurrentPoolsManager = nullptr;
}

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!IsValid(ObjectPoolSettings))
	{
		UE_LOG(LogObjectPool, Error, TEXT("UObjectPoolSubsystem::OnWorldBeginPlay: ObjectPoolSettings not valid."));
		return;
	}
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogObjectPool, Error, TEXT("UObjectPoolSubsystem::OnWorldBeginPlay: World not valid."));
		return;
	}
	
	const AGameModeBase* GameMode = World->GetAuthGameMode();
	if (!IsValid(GameMode))
	{
		UE_LOG(LogObjectPool, Error, TEXT("UObjectPoolSubsystem::OnWorldBeginPlay: GameMode not valid."));
		return;
	}
	
	CurrentPoolsManager = NewObject<UPoolsManager>(this);
	if (!IsValid(CurrentPoolsManager))
	{
		UE_LOG(LogObjectPool, Error, TEXT("UObjectPoolSubsystem::OnWorldBeginPlay: Failed to create PoolsManager."));
		return;
	}

	const FPoolsManagersData* PoolsManagersData = FindPoolsManagersDataForGamemode(GameMode);
	if (!PoolsManagersData)
	{
		UE_LOG(LogObjectPool, Display, TEXT("UObjectPoolSubsystem::OnWorldBeginPlay: No Pools Managers Data found for game mode: %s"), *GameMode->GetName());
		return;
	}
	
	CurrentPoolsManager->Init(World, PoolsManagersData->PoolsData);
}

const FPoolsManagersData* UObjectPoolSubsystem::FindPoolsManagersDataForGamemode(const AGameModeBase* GameMode) const
{
	TSoftClassPtr<AGameModeBase> TargetGameModeClass = GameMode->GetClass();
	const FPoolsManagersData* PoolsManagersData = ObjectPoolSettings->PoolsManagers.FindByPredicate(
		[&TargetGameModeClass](const FPoolsManagersData& Data)
		{
			return Data.AllowedGamemodes.Contains(TargetGameModeClass);
		}
	);

	return PoolsManagersData;
}

bool UObjectPoolSubsystem::Check() const
{
	return IsValid(CurrentPoolsManager) && IsValid(ObjectPoolSettings);
}
