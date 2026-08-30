// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveBehaviours/InventorySaveBehaviour.h"
#include "InventorySaveBridge.h"

void UInventorySaveBehaviour::BeginPlay()
{
	const AActor* Actor = GetOwnerActor();
	if (!IsValid(Actor))
		return;

	Inventory = Cast<UInventoryBase>(Actor->FindComponentByClass<UInventoryBase>());
	Super::BeginPlay();
}

USaveDataBase* UInventorySaveBehaviour::CreateSaveDataInstance_Implementation()
{
	if (!Check())
		return nullptr;
	
	if (!SaveClass->IsChildOf(UInventoryBaseSaveData::StaticClass()))
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("InventorySaveBehaviour: SaveDataClass is not a child of InventoryBaseSaveData."));
		return nullptr;
	}

	UInventoryBaseSaveData* InventorySaveData = NewObject<UInventoryBaseSaveData>(this, SaveClass);
	InventorySaveData->Init(this);
	InventorySaveData->RegisterItemsNative();
	return InventorySaveData;
}

bool UInventorySaveBehaviour::Save_Implementation(USaveDataBase* SaveData)
{
	if (!Check())
		return false;
	
	UInventoryBaseSaveData* InventorySaveData = Cast<UInventoryBaseSaveData>(SaveData);
	if (!IsValid(InventorySaveData))
		return false;
	
	return InventorySaveData->SaveObjectData(Inventory);
}

bool UInventorySaveBehaviour::Load_Implementation(USaveDataBase* SaveData)
{
	if (!Check())
		return false;

	UInventoryBaseSaveData* InventorySaveData = Cast<UInventoryBaseSaveData>(SaveData);
	if (!IsValid(InventorySaveData))
		return false;
	
	return InventorySaveData->LoadObjectData(Inventory);
}

bool UInventorySaveBehaviour::Check() const
{
	return IsValid(Inventory) && Super::Check();
}
