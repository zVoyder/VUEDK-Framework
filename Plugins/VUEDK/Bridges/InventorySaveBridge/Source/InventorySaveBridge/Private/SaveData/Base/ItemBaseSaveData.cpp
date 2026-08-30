// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/Base/ItemBaseSaveData.h"
#include "InventorySaveBridge.h"
#include "Base/ItemBase.h"
#include "Base/Data/ItemDataBase.h"

bool UItemBaseSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const UItemBase* Item = Cast<UItemBase>(ObjectToSave);

	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UItemBaseSaveData::SaveObjectData: Invalid object passed to save. "));
		return false;
	}

	SavedItemDataID = Item->GetItemData()->ItemDataId;
	SavedQuantity = Item->GetCurrentQuantity();
	SavedEquipSlotIndex = Item->GetEquipSlotIndex();
	return Super::SaveObjectData(ObjectToSave);
}

bool UItemBaseSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	UItemBase* Item = Cast<UItemBase>(ObjectToLoad);

	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UItemBaseSaveData::LoadObjectData: Invalid object passed to load. "));
		return false;
	}

	Item->SetQuantity(SavedQuantity);
	Item->SetEquipSlotIndex(SavedEquipSlotIndex);
	return Super::LoadObjectData(ObjectToLoad);
}

bool UItemBaseSaveData::PostLoadObjectDataNative(UObject* ObjectToLoad)
{
	return PostLoadObjectData(ObjectToLoad);
}

bool UItemBaseSaveData::PostLoadObjectData_Implementation(UObject* ObjectToLoad)
{
	return true;
}
