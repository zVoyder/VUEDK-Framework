// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/Base/InventoryBaseSaveData.h"
#include "InventorySaveBridge.h"
#include "Base/ItemBase.h"
#include "Factories/ISFactory.h"
#include "SaveBehaviours/InventorySaveBehaviour.h"
#include "Utility/ISInventoriesUtility.h"

void UInventoryBaseSaveData::Init(UInventorySaveBehaviour* InInventorySaveBehaviour)
{
	InventorySaveBehaviour = InInventorySaveBehaviour;
}

bool UInventoryBaseSaveData::SaveObjectData(UObject* ObjectToSave)
{
	UInventoryBase* Inventory = Cast<UInventoryBase>(ObjectToSave);
	if (!IsValid(Inventory))
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UInventoryBaseSaveData::SaveObjectNative: ObjectToSave is not of type UInventoryBase."));
		return false;
	}

	SavedItemsData.Empty();
	for (UItemBase* Item : Inventory->GetItems())
	{
		if (!IsValid(Item))
			continue;

		UItemBaseSaveData* ItemSaveData = CreateItemSaveData(Item);
		if (!IsValid(ItemSaveData))
			continue;

		SaveItem(Item, ItemSaveData);
		PostSaveItem(Item, ItemSaveData);
		SavedItemsData.Add(ItemSaveData);
	}

	SavedMaxWeight = Inventory->WeightMaxCapacity;
	return Super::SaveObjectData(ObjectToSave);
}

bool UInventoryBaseSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	UInventoryBase* Inventory = Cast<UInventoryBase>(ObjectToLoad);
	if (!IsValid(Inventory))
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UInventoryBaseSaveData::LoadObjectNative: ObjectToLoad is not of type UInventoryBase."));
		return false;
	}

	Inventory->ClearInventory();
	UEquipment* Equipment = Inventory->GetEquipment();
	if (IsValid(Equipment))
		Equipment->ClearEquipment();
	
	for (UItemBaseSaveData* ItemSaveData : SavedItemsData)
	{
		if (!IsValid(ItemSaveData))
			continue;

		UItemDataBase* ItemData = UISInventoriesUtility::GetItemDataFromRegistry(ItemSaveData->SavedItemDataID);
		if (!IsValid(ItemData))
			continue;

		UItemBase* Item = UISFactory::CreateSingleItem(Inventory, ItemData); // The quantity will be set in LoadItem
		if (!IsValid(Item))
			continue;

		LoadItem(Item, ItemSaveData);
		if (IsValid(Equipment) && Item->GetEquipSlotIndex() != -1)
			Equipment->TryEquipItem(Item, Item->GetEquipSlotKey(), Item->GetEquipSlotIndex());
		else
			Inventory->TryAddItem(Item);

		PostLoadItem(Item, ItemSaveData);
	}

	Inventory->WeightMaxCapacity = SavedMaxWeight;
	return Super::LoadObjectData(ObjectToLoad);
}

void UInventoryBaseSaveData::RegisterItemsNative()
{
	RegisterItemSaveData(UItemBase::StaticClass(), UItemBaseSaveData::StaticClass());
	RegisterItems();
}

UItemBaseSaveData* UInventoryBaseSaveData::CreateItemSaveData(const UItemBase* Item)
{
	if (!Check())
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UInventoryBaseSaveData::CreateItemSaveData: Check failed."));
		return nullptr;
	}

	UClass* ItemClass = Item->GetClass();
	const TSubclassOf<UItemBaseSaveData>* ItemSaveDataClass = nullptr;
	while (ItemClass && ItemClass->IsChildOf(UItemBase::StaticClass()))
	{
		ItemSaveDataClass = ItemToSaveDataMap.Find(ItemClass);
		if (ItemSaveDataClass && *ItemSaveDataClass)
			break;

		ItemClass = ItemClass->GetSuperClass();
	}

	if (!ItemSaveDataClass || !(*ItemSaveDataClass))
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UInventoryBaseSaveData::CreateItemSaveData: No ItemSaveDataClass registered for Item class %s."), *Item->GetClass()->GetName());
		return nullptr;
	}

	UItemBaseSaveData* ItemSaveData = NewObject<UItemBaseSaveData>(this, *ItemSaveDataClass);
	ItemSaveData->SetSaveDataID(InventorySaveBehaviour->GetCompositeSaveBehaviourID());
	return ItemSaveData;
}

void UInventoryBaseSaveData::RegisterItemSaveData(TSubclassOf<UItemBase> ItemClass, TSubclassOf<UItemBaseSaveData> ItemSaveDataClass)
{
	if (!ItemClass || !ItemSaveDataClass)
	{
		UE_LOG(LogInventorySaveBridge, Warning, TEXT("UInventoryBaseSaveData::RegisterItemSaveData: ItemClass or ItemSaveDataClass is null."));
		return;
	}

	ItemToSaveDataMap.Add(ItemClass, ItemSaveDataClass);
}

void UInventoryBaseSaveData::SaveItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
	ItemSaveData->SaveObjectData(Item);
	SaveItemNative(Item, ItemSaveData);
	ReceiveSaveItem(Item, ItemSaveData);
}

void UInventoryBaseSaveData::PostSaveItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
	PostSaveItemNative(Item, ItemSaveData);
	ReceivePostSaveItem(Item, ItemSaveData);
}

void UInventoryBaseSaveData::LoadItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
	ItemSaveData->LoadObjectData(Item);
	LoadItemNative(Item, ItemSaveData);
	ReceiveLoadItem(Item, ItemSaveData);
}

void UInventoryBaseSaveData::PostLoadItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
	ItemSaveData->PostLoadObjectDataNative(Item);
	PostLoadItemNative(Item, ItemSaveData);
	ReceivePostLoadItem(Item, ItemSaveData);
}

void UInventoryBaseSaveData::SaveItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
}

void UInventoryBaseSaveData::PostSaveItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
}

void UInventoryBaseSaveData::LoadItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
}

void UInventoryBaseSaveData::PostLoadItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
}

bool UInventoryBaseSaveData::Check() const
{
	return IsValid(InventorySaveBehaviour);
}
