// Copyright VUEDK, Inc. All Rights Reserved.

#include "TetrisInventory/TetrisItem.h"
#include "TetrisInventory/TetrisInventory.h"
#include "TetrisInventory/Data/TetrisItemData.h"
#include "TetrisInventory/Data/SaveData/TetrisItemSaveData.h"

UTetrisItem::UTetrisItem(): TopLeftCornerPosition(),
                            bIsRotated(false),
                            CachedSize(),
                            bCachedRotation(false)
{
}

void UTetrisItem::Init(UObject* WorldContextObject, UItemDataBase* Data)
{
	Super::Init(WorldContextObject, Data);
	CachedSize = GetTetrisItemData()->Size;
}

FTetrisItemSaveData UTetrisItem::CreateTetrisSaveData() const
{
	FTetrisItemSaveData TetrisSaveData;
	TetrisSaveData.ItemSaveData = CreateItemBaseSaveData();
	TetrisSaveData.SlotPosition = TopLeftCornerPosition;
	TetrisSaveData.bIsRotated = bIsRotated;
	return TetrisSaveData;
}

void UTetrisItem::LoadTetrisSaveData(UInventoryBase* LoadingInventory, const FTetrisItemSaveData& TetrisSaveData)
{
	bool bHasBeenEquipped;
	LoadItemBaseSaveData(LoadingInventory, TetrisSaveData.ItemSaveData, bHasBeenEquipped);
	UTetrisInventory* TetrisInventory = Cast<UTetrisInventory>(LoadingInventory);

	if (bHasBeenEquipped)
		return;

	SetRotation(TetrisSaveData.bIsRotated);
	TetrisInventory->TryMoveItem(this, TetrisSaveData.SlotPosition); // Since the item is already added in LoadItemBaseSaveData
}

bool UTetrisItem::CanRotate() const
{
	return GetTetrisItemData()->bCanBeRotated && GetTetrisItemData()->Size.X != GetTetrisItemData()->Size.Y;
}

void UTetrisItem::SetRotation(const bool bNewRotation)
{
	if (!CanRotate())
		return;

	bIsRotated = bNewRotation;
	OnItemRotated.Broadcast(GetRelativeSize());
}

void UTetrisItem::ResetToCachedRotation()
{
	SetRotation(bCachedRotation);
}

void UTetrisItem::Rotate()
{
	SetRotation(!bIsRotated);
}

UTetrisItemData* UTetrisItem::GetTetrisItemData() const
{
	return Cast<UTetrisItemData>(ItemData);
}

FIntPoint UTetrisItem::GetCurrentPosition() const
{
	return TopLeftCornerPosition;
}

FIntPoint UTetrisItem::GetCachedSize() const
{
	return CachedSize;
}

bool UTetrisItem::GetCachedRotation() const
{
	return bCachedRotation;
}

FIntPoint UTetrisItem::GetRelativeSize() const
{
	const FIntPoint Size = GetTetrisItemData()->Size;
	return bIsRotated ? FIntPoint(Size.Y, Size.X) : Size;
}

bool UTetrisItem::IsRotated() const
{
	return bIsRotated;
}

void UTetrisItem::SetCurrentPosition(const FIntPoint NewPosition)
{
	TopLeftCornerPosition = NewPosition;
}

void UTetrisItem::CacheCurrentRotation()
{
	bCachedRotation = bIsRotated;
	CachedSize = GetRelativeSize();
}

void UTetrisItem::OnEquip_Implementation()
{
	Super::OnEquip_Implementation();
	SetRotation(false);
}
