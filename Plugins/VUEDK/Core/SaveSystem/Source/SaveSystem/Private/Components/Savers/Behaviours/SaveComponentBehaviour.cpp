// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/Savers/Behaviours/SaveComponentBehaviour.h"

void USaveComponentBehaviour::BeginPlay()
{
	Super::BeginPlay();
	CachedComponent = FindComponent();
}

bool USaveComponentBehaviour::Save_Implementation(USaveDataBase* SaveData)
{
	return SaveData->SaveObjectData(CachedComponent);
}

bool USaveComponentBehaviour::Load_Implementation(USaveDataBase* SaveData)
{
	return SaveData->LoadObjectData(CachedComponent);
}

UActorComponent* USaveComponentBehaviour::FindComponent()
{
	const AActor* Actor = GetOwnerActor();

	if (!IsValid(Actor) || !IsValid(ComponentClass))
		return nullptr;

	return Actor->FindComponentByClass(ComponentClass);
}
