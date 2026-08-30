// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/BloodDecalsPool.h"
#include "DismembererSystem.h"
#include "Effects/BloodDecal.h"
#include "GameFramework/GameModeBase.h"

void UBloodDecalsPool::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DismembererSystemSettings = GetDefault<UDismembererSystemSettings>();
}


void UBloodDecalsPool::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	if (AreDecalsAllowedForWorld(InWorld))
		SpawnInstances();
}

void UBloodDecalsPool::DestroyPool()
{
	for (AActor* Actor : AvailableDecals)
	{
		if (IsValid(Actor))
			Actor->Destroy();
	}

	for (AActor* Actor : SpawnedDecals)
	{
		if (IsValid(Actor))
			Actor->Destroy();
	}

	AvailableDecals.Empty();
	SpawnedDecals.Empty();
}

ABloodDecal* UBloodDecalsPool::AcquireDecal()
{
	if (IsEmpty())
		SpawnInstance();

	return PopDecal();
}

void UBloodDecalsPool::ReleaseDecal(ABloodDecal* InActor)
{
	if (!IsValid(InActor))
		return;

	if (AvailableDecals.Contains(InActor))
		return;

	PushDecal(InActor);
}

bool UBloodDecalsPool::AreDecalsAllowedForWorld(const UWorld& InWorld) const
{
	if (!IsValid(DismembererSystemSettings))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UBloodDecalsPool::HasAllowedDecals: DismembererSystemSettings not valid."));
		return true;
	}

	const AGameModeBase* GameMode = InWorld.GetAuthGameMode();
	if (!IsValid(GameMode))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UBloodDecalsPool::HasAllowedDecals: GameMode not valid."));
		return true;
	}

	const TSoftClassPtr<AGameModeBase> GameModeClassPtr = GameMode->GetClass();
	return DismembererSystemSettings->AllowedDecalGamemodes.Contains(GameModeClassPtr);
}

void UBloodDecalsPool::SpawnInstances()
{
	if (!IsValid(DismembererSystemSettings))
		return;

	const float Size = DismembererSystemSettings->DecalPoolSize;
	for (int32 i = 0; i < Size; i++)
		SpawnInstance();
}

void UBloodDecalsPool::SpawnInstance()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	const UDismembererSystemSettings* Settings = GetDefault<UDismembererSystemSettings>();
	if (!IsValid(Settings))
		return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bNoFail = true;
	ABloodDecal* Actor = World->SpawnActor<ABloodDecal>(Settings->BloodDecal.LoadSynchronous(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (!IsValid(Actor))
		return;

	Actor->PrimaryActorTick.bStartWithTickEnabled = false;
	PushDecal(Actor);
}

void UBloodDecalsPool::PushDecal(ABloodDecal* Actor)
{
	ClearDecal(Actor);
	SetDecalEnabled(Actor, false);
	AvailableDecals.Push(Actor);
	SpawnedDecals.Remove(Actor);
}

ABloodDecal* UBloodDecalsPool::PopDecal()
{
	const int LastIndex = AvailableDecals.Num() - 1;
	if (!AvailableDecals.IsValidIndex(LastIndex))
		return nullptr;

	ABloodDecal* Actor = AvailableDecals[LastIndex];
	if (!IsValid(Actor))
		return nullptr;

	SetDecalEnabled(Actor, true);
	SpawnedDecals.Add(Actor);
	AvailableDecals.Remove(Actor);
	return Actor;
}

bool UBloodDecalsPool::IsEmpty() const
{
	return AvailableDecals.Num() == 0;
}

void UBloodDecalsPool::SetDecalEnabled(ABloodDecal* Actor, const bool bIsEnabled)
{
	if (!IsValid(Actor))
		return;

	bIsEnabled ? Actor->RegisterAllComponents() : Actor->UnregisterAllComponents();
	Actor->SetActorHiddenInGame(!bIsEnabled);
	Actor->SetActorEnableCollision(bIsEnabled);
	Actor->SetActorTickEnabled(bIsEnabled);

#if WITH_EDITOR
	FString CategoryName = "BloodsDecal";
	FString Name;
	const FString ActorEnabled = Name.Append(CategoryName).Append(bIsEnabled ? TEXT("_InUse") : TEXT("_Available"));
	Actor->SetActorLabel(ActorEnabled, false);
	const FString FolderPathString = "Pools/Pool_" + CategoryName;
	const FName FolderPathName = FName(*FolderPathString);
	Actor->SetFolderPath(FolderPathName);
#endif
}

void UBloodDecalsPool::ClearDecal(ABloodDecal* Actor)
{
	if (!IsValid(Actor))
		return;

	Actor->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
}
