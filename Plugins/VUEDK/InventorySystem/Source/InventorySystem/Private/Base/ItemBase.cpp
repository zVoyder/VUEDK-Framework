// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/ItemBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Base/InventoryBase.h"
#include "Base/Data/ItemDataBase.h"
#include "Base/Data/SaveData/ItemSaveData.h"
#include "Factories/ISFactory.h"

UItemBase::UItemBase(): RelatedInventory(nullptr),
                        RelatedEquipment(nullptr),
                        ItemData(nullptr),
                        EquipSlotIndex(-1),
                        CurrentQuantity(1),
                        ItemWorldContext(nullptr)
{
}

FItemSaveData UItemBase::CreateItemBaseSaveData() const
{
	FItemSaveData ItemSaveData;
	ItemSaveData.EquipSlotIndex = EquipSlotIndex;
	ItemSaveData.Quantity = CurrentQuantity;
	return ItemSaveData;
}

void UItemBase::LoadItemBaseSaveData(UInventoryBase* LoadingInventory, const FItemSaveData ItemSaveData, bool& bOutHasBeenEquipped)
{
	bOutHasBeenEquipped = false;
	EquipSlotIndex = ItemSaveData.EquipSlotIndex;
	CurrentQuantity = ItemSaveData.Quantity;

	if (EquipSlotIndex == -1)
	{
		if (!LoadingInventory->TryAddItem(this))
			UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::LoadItemBaseSaveData, Item %s couldn't be added to the inventory %s."), *ItemData->ItemTypeID, *LoadingInventory->GetName());

		return;
	}

	if (!LoadingInventory->GetEquipment()->TryEquipItem(this, ItemData->EquipSlotKey, EquipSlotIndex))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::LoadItemBaseSaveData, Item %s couldn't be equipped to the equipment %s."), *ItemData->ItemTypeID, *LoadingInventory->GetEquipment()->GetName());
		ClearEquipSlot();
		return;
	}

	bOutHasBeenEquipped = true;
}

void UItemBase::Init(UObject* WorldContextObject, UItemDataBase* Data)
{
	ItemWorldContext = WorldContextObject;
	OnPreInit(Data);
	ItemData = Data;
	SetItemMeshToLoad();
	OnInit();
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Init, Item %s has been initialized."), *ItemData->ItemTypeID);
}

UItemDataBase* UItemBase::GetItemData() const
{
	return ItemData;
}

void UItemBase::AssignInventory(UInventoryBase* Inventory)
{
	RelatedInventory = Inventory;
}

void UItemBase::DeassignInventory()
{
	RelatedInventory = nullptr;
}

void UItemBase::SetEquipSlot(UEquipment* InEquipment, const int32 SlotIndex)
{
	RelatedEquipment = InEquipment;
	EquipSlotIndex = SlotIndex;
	OnItemEquipped.Broadcast(this);
	OnEquip();
}

void UItemBase::ClearEquipSlot()
{
	RelatedEquipment = nullptr;
	EquipSlotIndex = -1;
	OnItemUnequipped.Broadcast(this);
	OnUnequip();
}

bool UItemBase::IsEquipped() const
{
	return IsValid(RelatedEquipment) && EquipSlotIndex > -1;
}

bool UItemBase::TryDrop(const FVector Location, const FRotator Rotation, AItemDropActor*& OutItemDropActor, const bool bNotify)
{
	if (!CanDrop())
		return false;

	OutItemDropActor = UISFactory::CreateItemDropActor(ItemWorldContext, this);

	if (!IsValid(OutItemDropActor))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::Drop(), ItemDropActor is nullptr."));
		return false;
	}

	OutItemDropActor->SetActorLocationAndRotation(Location, Rotation);
	Remove();

	if (IsEquipped())
		RelatedEquipment->TryUnequipItem(this);

	OnDrop(OutItemDropActor);
	OnItemDropped.Broadcast(this, OutItemDropActor);

	if (bNotify)
	{
		OnDropNotify(OutItemDropActor);
		OnItemDroppedNotify.Broadcast(this, OutItemDropActor);
	}
	
	return true;
}

void UItemBase::Remove()
{
	if (!IsValid(RelatedInventory))
		return;

	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Remove(), Item %s has been removed from the inventory %s."), *ItemData->ItemTypeID, *RelatedInventory->GetName());
	RelatedInventory->RemoveItem(this);
}

void UItemBase::Use()
{
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Use(), Item %s has been used."), *ItemData->ItemTypeID);

	OnItemUsed.Broadcast(this);
	RelatedInventory->OnAnyItemUsed.Broadcast(this);
	OnUse();

	if (ItemData->bConsumeUponUse)
		Consume();
}

int32 UItemBase::Consume(const int32 AmountToConsume)
{
	if (!ItemData->bIsConsumable)
		return 0;

	UE_LOG(LogInventorySystem, Display, TEXT("Consuming Item %s, Current Quantity: %d, Amount To Consume: %d."), *ItemData->ItemTypeID, CurrentQuantity, AmountToConsume);

	RelatedInventory->OnAnyItemConsumed.Broadcast(this);
	OnItemConsumed.Broadcast(this);
	OnConsume_Implementation();
	const int32 Rest = DecreaseQuantity(AmountToConsume);
	return Rest;
}

FText UItemBase::GetItemFullName() const
{
	return ItemData->ItemName;
}

FSlateBrush UItemBase::GetItemIcon() const
{
	return ItemData->ItemIcon;
}

FText UItemBase::GetItemDescription() const
{
	return ItemData->ItemDescription;
}

UStaticMesh* UItemBase::LoadItemMeshSync() const
{
	if (ItemMesh.IsValid())
	{
		OnItemMeshLoaded.Broadcast(ItemMesh.Get());
		return ItemMesh.Get();
	}

	UStaticMesh* Mesh = ItemMesh.LoadSynchronous();
	OnItemMeshLoaded.Broadcast(Mesh);
	return Mesh;
}

void UItemBase::LoadItemMeshAsync() const
{
	if (ItemMesh.IsValid())
	{
		OnItemMeshLoaded.Broadcast(ItemMesh.Get());
		return;
	}

	UAssetManager& AssetManager = UAssetManager::Get();
	AssetManager.GetStreamableManager().RequestAsyncLoad(ItemMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UItemBase::OnAsyncLoadItemMesh));
}

float UItemBase::GetItemWeight_Implementation() const
{
	return ItemData->ItemWeight;
}

float UItemBase::GetItemFullWeight_Implementation() const
{
	return GetItemWeight() * CurrentQuantity;
}

int32 UItemBase::GetEquipSlotIndex() const
{
	return EquipSlotIndex;
}

UEquipSlotKey* UItemBase::GetEquipSlotKey() const
{
	return ItemData->EquipSlotKey;
}

bool UItemBase::CanStack() const
{
	return IsStackable() && !IsMaxStacked();
}

bool UItemBase::IsEquippable()
{
	return HasEquipPermission() && AreEquipPreconditionsMet();
}

bool UItemBase::HasEquipPermission_Implementation() const
{
	return true;
}

bool UItemBase::AreEquipPreconditionsMet()
{
	if (!ItemData->bUsePreconditions)
		return true;

	switch (ItemData->PreconditionsMode)
	{
	case All:
		for (const UEquipPrecondition* Precondition : ItemData->EquipPreconditions)
		{
			if (!IsValid(Precondition) || !Precondition->IsPreconditionMet(this, RelatedEquipment))
				return false;
		}
		return true;
	case Any:
		for (const UEquipPrecondition* Precondition : ItemData->EquipPreconditions)
		{
			if (IsValid(Precondition) && Precondition->IsPreconditionMet(this, RelatedEquipment))
				return true;
		}
		return false;
	default: ;
	}

	return false;
}

bool UItemBase::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	return
		OtherItem != this &&
		IsValid(OtherItem) &&
		OtherItem->ItemData == ItemData &&
		CanStack();
}

bool UItemBase::IsMaxStacked() const
{
	return CurrentQuantity >= ItemData->MaxStackSize;
}

bool UItemBase::IsStackable() const
{
	return ItemData->MaxStackSize > 1;
}

int32 UItemBase::GetCurrentQuantity() const
{
	return CurrentQuantity;
}

bool UItemBase::TryStackItem(UItemBase* Item, const int32 AmountToStack)
{
	if (AmountToStack <= 0 || !CanStackItem(Item))
		return false;

	if (Item->GetCurrentQuantity() - AmountToStack < 0 || GetCurrentQuantity() + AmountToStack > ItemData->MaxStackSize)
		return false;

	IncreaseQuantity(AmountToStack);
	Item->DecreaseQuantity(AmountToStack);

	return true;
}

bool UItemBase::TrySplitItem(const int32 AmountToSplit, UItemBase*& OutNewSplittedItem)
{
	OutNewSplittedItem = nullptr;

	if (AmountToSplit <= 0 || AmountToSplit >= CurrentQuantity || !IsStackable())
		return false;

	OutNewSplittedItem = DuplicateItem();
	if (OutNewSplittedItem == nullptr)
		return false;

	OutNewSplittedItem->SetQuantity(AmountToSplit);
	DecreaseQuantity(AmountToSplit);

	return true;
}

int32 UItemBase::IncreaseQuantity(const int32 Amount)
{
	if (!CanStack())
		return 0;

	if (CurrentQuantity + Amount > ItemData->MaxStackSize)
	{
		const int32 Overflow = CurrentQuantity + Amount - ItemData->MaxStackSize;
		CurrentQuantity = ItemData->MaxStackSize;
		RelatedInventory->AddWeight(GetItemWeight() * (Amount - Overflow));
		OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
		return Overflow;
	}

	CurrentQuantity += Amount;
	RelatedInventory->AddWeight(GetItemWeight() * (Amount));
	OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
	return 0;
}

int32 UItemBase::DecreaseQuantity(const int32 Amount)
{
	if (CurrentQuantity - Amount > 0)
	{
		CurrentQuantity -= Amount;
		RelatedInventory->RemoveWeight(GetItemWeight() * Amount);
		OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
		return 0;
	}

	const int32 Rest = Amount - CurrentQuantity;
	CurrentQuantity = 0;
	RelatedInventory->RemoveWeight(GetItemWeight() * (Amount - Rest));
	OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
	Remove();
	return Rest;
}

int32 UItemBase::SetQuantity(const int32 Quantity)
{
	if (!IsStackable())
		return 0;

	CurrentQuantity = FMath::Clamp(Quantity, 0, ItemData->MaxStackSize);

	if (CurrentQuantity <= 0)
		Remove();

	OnItemQuantityChanged.Broadcast(this, CurrentQuantity);

	int32 Rest = Quantity - ItemData->MaxStackSize;
	Rest = Rest < 0 ? 0 : Rest;
	return Rest;
}

UItemBase* UItemBase::DuplicateItem() const
{
	return DuplicateObject<UItemBase>(this, GetOuter());
}

void UItemBase::OnPreInit(UItemDataBase* Data)
{
}

void UItemBase::OnInit_Implementation()
{
}

void UItemBase::OnAdd_Implementation()
{
}

void UItemBase::OnRemove_Implementation()
{
}

void UItemBase::OnUse_Implementation()
{
}

void UItemBase::OnConsume_Implementation()
{
}

void UItemBase::OnEquip_Implementation()
{
}

void UItemBase::OnUnequip_Implementation()
{
}

void UItemBase::OnDrop_Implementation(AItemDropActor* ItemDropActor)
{
}

void UItemBase::OnDropNotify_Implementation(AItemDropActor* ItemDropActor)
{
}

UWorld* UItemBase::GetWorld() const
{
	if (!IsValid(ItemWorldContext))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::GetWorld: ItemWorldContext is nullptr."));
		return nullptr;
	}

	return ItemWorldContext->GetWorld();
}

#if WITH_EDITOR
bool UItemBase::ImplementsGetWorld() const
{
	return true;
}
#endif

void UItemBase::SetItemMeshToLoad()
{
	ItemMesh = ItemData->ItemMesh;
}

bool UItemBase::CanDrop() const
{
	return ItemData->ItemDropActorClass != nullptr;
}

void UItemBase::OnAsyncLoadItemMesh() const
{
	if (!ItemMesh.IsValid())
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::OnAsyncLoadItemMesh, ItemMesh is nullptr."));
		return;
	}

	OnItemMeshLoaded.Broadcast(ItemMesh.Get());
}
