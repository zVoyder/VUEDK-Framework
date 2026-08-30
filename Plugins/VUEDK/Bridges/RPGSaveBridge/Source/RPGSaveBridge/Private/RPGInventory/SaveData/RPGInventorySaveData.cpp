// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/SaveData/RPGInventorySaveData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "RPGInventory/SaveData/RPGGearItemSaveData.h"
#include "RPGInventory/SaveData/RPGItemSaveData.h"

void URPGInventorySaveData::RegisterItemsNative()
{
	Super::RegisterItemsNative();
	RegisterItemSaveData(URPGItem::StaticClass(), URPGItemSaveData::StaticClass());
	RegisterItemSaveData(URPGGearItem::StaticClass(), URPGGearItemSaveData::StaticClass());
}
