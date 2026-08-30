// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/ItemBase.h"
#include "InventorySystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Base/InventoryBase.h"
#include "Base/Data/ItemDataBase.h"
#include "Factories/ISFactory.h"

UItemBase::UItemBase() : RelatedInventory(nullptr),
                         RelatedEquipment(nullptr),
                         ItemData(nullptr),
                         EquipSlotIndex(-1),
                         CurrentQuantity(1)
{
}

void UItemBase::Init(const TObjectPtr<UWorld> InWorld, UItemDataBase* Data)
{
	if (!IsValid(Data))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::Init: ItemData is nullptr."));
		return;
	}

	OwningWorld = InWorld;
	OnPreInit(Data);
	ItemData = Data;
	SetItemMeshToLoad();
	OnInit();
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Init: Item %s has been initialized."), *ItemData->ItemTypeId);
}

UItemDataBase* UItemBase::GetItemData() const
{
	return ItemData;
}

void UItemBase::AssignInventory(UInventoryBase* Inventory)
{
	RelatedInventory = Inventory;
}

void UItemBase::ClearInventory()
{
	RelatedInventory = nullptr;
}

void UItemBase::SetEquipSlot(UEquipment* InEquipment, const int32 SlotIndex)
{
	RelatedEquipment = InEquipment;
	SetEquipSlotIndex(SlotIndex);
	OnItemEquipped.Broadcast(this);
	OnEquip();
}

void UItemBase::SetEquipSlotIndex(const int32 SlotIndex)
{
	EquipSlotIndex = SlotIndex;
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

bool UItemBase::TryDrop(const UObject* WorldContextObject, const FVector Location, const FRotator Rotation, AItemDropActor*& OutItemDropActor, const bool bNotify)
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::Drop: Check failed."));
		return false;
	}

	if (!CanDrop())
		return false;

	OutItemDropActor = UISFactory::CreateItemDropActor(WorldContextObject, this);

	if (!IsValid(OutItemDropActor))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::Drop: ItemDropActor is nullptr."));
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

	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Remove: Item %s has been removed from the inventory %s."), *ItemData->ItemTypeId, *RelatedInventory->GetName());
	RelatedInventory->RemoveItem(this);
}

void UItemBase::Use()
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::Use: Check failed."));
		return;
	}

	OnItemUsed.Broadcast(this);
	if (IsValid(RelatedInventory))
		RelatedInventory->OnAnyItemUsed.Broadcast(this);
	
	OnUse();
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Use: Item %s has been used."), *ItemData->ItemTypeId);

	if (ItemData->bConsumeUponUse)
		Consume();
}

int32 UItemBase::Consume(const int32 AmountToConsume)
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::Consume: Check failed."));
		return 0;
	}

	if (!ItemData->bIsConsumable)
		return 0;

	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Consume: Consuming Item %s, Current Quantity: %d, Amount To Consume: %d."), *ItemData->ItemTypeId, CurrentQuantity, AmountToConsume);
	if (IsValid(RelatedInventory))
		RelatedInventory->OnAnyItemConsumed.Broadcast(this);
	
	OnItemConsumed.Broadcast(this);
	OnConsume_Implementation();
	const int32 Rest = DecreaseQuantity(AmountToConsume);
	return Rest;
}

FText UItemBase::GetItemFullName() const
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::GetItemFullName: Check failed."));
		return FText::FromString("Invalid Item");
	}

	return ItemData->ItemName;
}

FSlateBrush UItemBase::GetItemIcon() const
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::GetItemIcon: Check failed."));
		return FSlateBrush();
	}

	return ItemData->ItemIcon;
}

FText UItemBase::GetItemDescription() const
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::GetItemDescription: Check failed."));
		return FText::FromString("Invalid Item");
	}

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
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::GetItemWeight: Check failed."));
		return 0.f;
	}

	return ItemData->ItemWeight;
}

float UItemBase::GetItemFullWeight() const
{
	return GetItemWeight() * CurrentQuantity;
}

int32 UItemBase::GetEquipSlotIndex() const
{
	return EquipSlotIndex;
}

UEquipSlotKey* UItemBase::GetEquipSlotKey() const
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::GetEquipSlotKey: Check failed."));
		return nullptr;
	}

	return ItemData->EquipSlotKey;
}

bool UItemBase::CanStack() const
{
	return IsStackable() && !IsMaxStacked();
}

bool UItemBase::CanEquip()
{
	return HasEquipPermission() && AreEquipPreconditionsMet();
}

bool UItemBase::HasEquipPermission_Implementation() const
{
	return true;
}

bool UItemBase::AreEquipPreconditionsMet()
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::AreEquipPreconditionsMet: Check failed."));
		return false;
	}

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
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::CanStackItem: Check failed."));
		return false;
	}

	return
		OtherItem != this &&
		IsValid(OtherItem) &&
		OtherItem->ItemData == ItemData &&
		CanStack();
}

bool UItemBase::IsMaxStacked() const
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::IsMaxStacked: Check failed."));
		return false;
	}

	return CurrentQuantity >= ItemData->MaxStackSize;
}

bool UItemBase::IsStackable() const
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::IsStackable: Check failed."));
		return false;
	}

	return ItemData->MaxStackSize > 1;
}

int32 UItemBase::GetCurrentQuantity() const
{
	return CurrentQuantity;
}

bool UItemBase::TryStackItem(UItemBase* Item, const int32 AmountToStack)
{
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::TryStackItem: Check failed."));
		return false;
	}

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
	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::SetQuantity: Check failed."));
		return 0;
	}

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

bool UItemBase::Check() const
{
	return IsValid(ItemData);
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
	UWorld* World = OwningWorld.Get();
	if (IsValid(World))
		return World;

	if (IsValid(RelatedInventory))
	{
		World = RelatedInventory->GetWorld();
		if (IsValid(World))
		{
			OwningWorld = World;
			return World;
		}
	}

	if (GEngine)
	{
		World = GEngine->GetWorldContexts().Num() > 0 ? GEngine->GetWorldContexts()[0].World() : nullptr;
		if (IsValid(World))
		{
			OwningWorld = World;
			return World;
		}
	}
	
	UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::GetWorld: Could not find a valid UWorld."));
	return nullptr;
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
		UE_LOG(LogInventorySystem, Warning, TEXT("ItemBase::OnAsyncLoadItemMesh: ItemMesh is nullptr."));
		return;
	}

	OnItemMeshLoaded.Broadcast(ItemMesh.Get());
}
