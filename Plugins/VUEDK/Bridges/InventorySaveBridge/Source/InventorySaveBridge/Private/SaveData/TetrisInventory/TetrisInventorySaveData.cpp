// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/TetrisInventory/TetrisInventorySaveData.h"
#include "SaveData/TetrisInventory/TetrisItemSaveData.h"
#include "TetrisInventory/TetrisItem.h"

void UTetrisInventorySaveData::RegisterItemsNative()
{
	Super::RegisterItemsNative();
	RegisterItemSaveData(UTetrisItem::StaticClass(), UTetrisItemSaveData::StaticClass());
}

void UTetrisInventorySaveData::PostLoadItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData)
{
	Super::PostLoadItemNative(Item, ItemSaveData);

	if (Item->IsEquipped())
		return;

	const UTetrisItemSaveData* TetrisItemSaveData = Cast<UTetrisItemSaveData>(ItemSaveData);
	if (!IsValid(TetrisItemSaveData))
		return;

	UTetrisItem* TetrisItem = Cast<UTetrisItem>(Item);
	if (!IsValid(TetrisItem))
		return;

	TetrisItem->SetRotation(TetrisItemSaveData->bIsRotated);
	UTetrisInventory* TetrisItemInventory = Cast<UTetrisInventory>(TetrisItem->RelatedInventory);
	if (!IsValid(TetrisItemInventory))
		return;

	TetrisItemInventory->TryMoveItem(TetrisItem, TetrisItemSaveData->SlotPosition);
}
