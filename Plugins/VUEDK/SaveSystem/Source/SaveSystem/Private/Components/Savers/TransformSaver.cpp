// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/Savers/TransformSaver.h"
#include "SaveManager.h"
#include "Utility/SSUtility.h"

UTransformSaver::UTransformSaver()
{
}

void UTransformSaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	if (!USSUtility::GetSaveManager()) return;
	USSUtility::GetSaveManager()->GetSaveGameInstance()->ActorTransforms.Add(GetUniqueSaveID(), GetOwnerTransform());
}

void UTransformSaver::BeginPlay()
{
	Super::BeginPlay();
	SetActorLoadedTransform(SaveManager->GetSaveGameInstance());
}

void UTransformSaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	Super::OnLoadCompletedEvent_Implementation(SlotName, UserIndex, LoadedData, Instigator);
	SetActorLoadedTransform(LoadedData);
}

FTransform UTransformSaver::GetOwnerTransform() const
{
	return GetOwner()->GetActorTransform();
}

void UTransformSaver::SetActorLoadedTransform(UDefaultSaveGame* LoadedData) const
{
	if (!LoadedData) return;
	
	// Check if LoadedData and ActorTransforms are valid
	if (const FName OwnerUniqueID = GetUniqueSaveID(); LoadedData && LoadedData->ActorTransforms.Contains(OwnerUniqueID))
	{
		// Check if Owner is valid
		if (AActor* Owner = GetOwner())
			Owner->SetActorTransform(LoadedData->ActorTransforms[OwnerUniqueID]);
		else
			UE_LOG(LogSaveSystem, Warning, TEXT("Owner not found in the loaded data, Saver Unique ID: %s"), *OwnerUniqueID.ToString());
	}
}
