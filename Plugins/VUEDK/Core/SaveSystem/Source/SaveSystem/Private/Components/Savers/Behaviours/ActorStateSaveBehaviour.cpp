// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/Savers/Behaviours/ActorStateSaveBehaviour.h"
#include "SaveManager.h"
#include "SaveSystem.h"
#include "Data/SaveData/ActorStateSaveData.h"
#include "Utility/SSUtility.h"

USaveDataBase* UActorStateSaveBehaviour::CreateSaveDataInstanceNative()
{
	UActorStateSaveData* SaveData = Cast<UActorStateSaveData>(Super::CreateSaveDataInstanceNative());

	if (!IsValid(SaveData))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("UActorStateSaveBehaviour::CreateSaveDataInstanceNative: Failed to create SaveData instance of type UActorStateSaveData."));
		return nullptr;
	}

	SaveData->Init(this);
	return SaveData;
}

bool UActorStateSaveBehaviour::Save_Implementation(USaveDataBase* SaveData)
{
	UActorStateSaveData* ActorStateSaveData = Cast<UActorStateSaveData>(SaveData);
	if (!IsValid(ActorStateSaveData))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("UTransformSaveBehaviour::Save_Implementation: SaveData is not of type UActorStateSaveData."));
		return false;
	}

	return ActorStateSaveData->SaveObjectData(GetOwnerActor());
}

bool UActorStateSaveBehaviour::Load_Implementation(USaveDataBase* SaveData)
{
	UActorStateSaveData* ActorStateSaveData = Cast<UActorStateSaveData>(SaveData);
	if (!IsValid(ActorStateSaveData))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("UTransformSaveBehaviour::Load_Implementation: SavedData is not of type UActorStateSaveData."));
		return false;
	}

	const UDefaultSaveGame* SaveGame = USSUtility::GetSaveGame();
	if (!IsValid(SaveGame))
		return false;

	if (SaveGame->DestroyedActors.Contains(GetCompositeSaveBehaviourID()))
	{
		if (AActor* OwnerActor = GetOwnerActor())
		{
			OwnerActor->Destroy();
			return true;
		}

		return false;
	}

	return ActorStateSaveData->LoadObjectData(GetOwnerActor());
}

void UActorStateSaveBehaviour::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!bMarkOnDestroy || EndPlayReason != EEndPlayReason::Type::Destroyed)
		return;

	MarkAsDestroyed();
}

void UActorStateSaveBehaviour::MarkAsDestroyed() const
{
	UDefaultSaveGame* SaveGame = USSUtility::GetSaveGame();
	if (!IsValid(SaveGame))
		return;

	SaveGame->DestroyedActors.AddUnique(GetCompositeSaveBehaviourID());
}

void UActorStateSaveBehaviour::MarkAsNotDestroyed() const
{
	UDefaultSaveGame* SaveGame = USSUtility::GetSaveGame();
	if (!IsValid(SaveGame))
		return;

	SaveGame->DestroyedActors.Remove(GetCompositeSaveBehaviourID());
}
