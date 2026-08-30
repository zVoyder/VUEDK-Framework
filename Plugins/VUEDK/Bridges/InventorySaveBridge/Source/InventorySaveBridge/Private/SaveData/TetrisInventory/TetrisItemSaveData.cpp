// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/TetrisInventory/TetrisItemSaveData.h"
#include "TetrisInventory/TetrisItem.h"

bool UTetrisItemSaveData::SaveObjectData(UObject* ObjectToSave)
{
	const UTetrisItem* TetrisItem = Cast<UTetrisItem>(ObjectToSave);
	if (!IsValid(TetrisItem))
		return false;

	SlotPosition = TetrisItem->GetCurrentPosition();
	bIsRotated = TetrisItem->IsRotated();
	return Super::SaveObjectData(ObjectToSave);
}
