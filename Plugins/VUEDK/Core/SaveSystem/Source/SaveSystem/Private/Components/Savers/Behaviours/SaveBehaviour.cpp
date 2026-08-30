// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/Savers/Behaviours/SaveBehaviour.h"
#include "Components/Savers/Saver.h"

USaveBehaviour::USaveBehaviour() : UniqueBehaviourID(FGuid::NewGuid())
{
}

void USaveBehaviour::Init(USaver* OwnerSaver)
{
	CachedOwnerSaver = OwnerSaver;
	CacheSaveDataInstanceIfNeeded();
}

void USaveBehaviour::BeginPlay()
{
	ReceiveBeginPlay();
}

void USaveBehaviour::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ReceiveEndPlay(EndPlayReason);
}

USaveDataBase* USaveBehaviour::CreateSaveDataInstanceNative()
{
	USaveDataBase* SaveData = Execute_CreateSaveDataInstance(this);
	if (IsValid(SaveData))
		SaveData->SetSaveDataID(GetCompositeSaveBehaviourID());

	return SaveData;
}

USaveDataBase* USaveBehaviour::CreateSaveDataInstance_Implementation()
{
	return NewObject<USaveDataBase>(this, SaveClass);
}

void USaveBehaviour::PrepareForSerializationNative(USaveDataBase* SaveData)
{
	Execute_PrepareForSerialization(this, SaveData);
}

void USaveBehaviour::PrepareForSerialization_Implementation(USaveDataBase* SaveData)
{
}

void USaveBehaviour::PrepareForDeserializationNative(USaveDataBase* SaveData)
{
	Execute_PrepareForDeserialization(this, SaveData);
}

void USaveBehaviour::PrepareForDeserialization_Implementation(USaveDataBase* SaveData)
{
}

bool USaveBehaviour::Save_Implementation(USaveDataBase* SaveData)
{
	return SaveData->SaveObjectData(this);
}

bool USaveBehaviour::Load_Implementation(USaveDataBase* SaveData)
{
	return SaveData->LoadObjectData(this);
}

USaveDataBase* USaveBehaviour::GetSaveDataInstance()
{
	if (bPreserveSaveDataInstance)
	{
		if (!IsValid(CachedSaveData))
			CachedSaveData = CreateSaveDataInstanceNative();

		return CachedSaveData;
	}

	return CreateSaveDataInstanceNative();
}

FName USaveBehaviour::GetCompositeSaveBehaviourID() const
{
	if (!Check())
		return NAME_None;

	const FName UniqueID = FName(*UniqueBehaviourID.ToString());
	return CachedOwnerSaver->MakeCompositeSaveID(UniqueID);
}

bool USaveBehaviour::Check() const
{
	return IsValid(CachedOwnerSaver) && SaveClass != nullptr;
}

#if WITH_EDITOR
bool USaveBehaviour::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* USaveBehaviour::GetWorld() const
{
	return GetOuter()->GetWorld();
}

USaver* USaveBehaviour::GetOwnerSaver()
{
	if (IsValid(CachedOwnerSaver))
		return CachedOwnerSaver;

	if (USaver* OwnerSaver = Cast<USaver>(GetOuter()))
	{
		CachedOwnerSaver = OwnerSaver;
		return CachedOwnerSaver;
	}

	return nullptr;
}

AActor* USaveBehaviour::GetOwnerActor()
{
	const USaver* OwnerSaver = GetOwnerSaver();

	if (!IsValid(OwnerSaver))
		return nullptr;

	return OwnerSaver->GetOwner();
}

void USaveBehaviour::CacheSaveDataInstanceIfNeeded()
{
	if (bPreserveSaveDataInstance)
	{
		if (!IsValid(CachedSaveData))
			CachedSaveData = CreateSaveDataInstanceNative();
	}
}
