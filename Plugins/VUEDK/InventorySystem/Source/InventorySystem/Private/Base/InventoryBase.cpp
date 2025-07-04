// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/InventoryBase.h"
#include "Base/ItemBase.h"
#include "Base/Data/SaveData/InventoryBaseSaveData.h"
#include "Factories/ISFactory.h"
#include "Utility/ISInventoriesUtility.h"

UInventoryBase::UInventoryBase(): bUseWeight(false),
                                  WeightMaxCapacity(0),
                                  RelatedEquipment(nullptr),
                                  CurrentWeight(0)
{
	PrimaryComponentTick.bCanEverTick = false;
}

USaveData* UInventoryBase::CreateSaveData()
{
	UE_LOG(LogInventorySystem, Display, TEXT("Creating Save Data for inventory %s."), *GetName());
	USaveData* SaveData = CreateSaveDataObject();
	TArray<UItemBase*> ItemsToSave = GetItems();
	return CreateInventorySaveData_Implementation(SaveData, ItemsToSave);
}

bool UInventoryBase::LoadSaveData(USaveData* SavedData)
{
	if (UInventoryBaseSaveData* InventorySaveData = Cast<UInventoryBaseSaveData>(SavedData))
	{
		// Clear the inventory before loading the save data
		if (IsValid(RelatedEquipment))
			RelatedEquipment->ClearEquipment();

		ClearInventory();
		LoadInventorySaveData(InventorySaveData);
		return true;
	}

	UE_LOG(LogInventorySystem, Error, TEXT("Invalid Save Data type for inventory %s."), *GetName());
	return false;
}

void UInventoryBase::LinkEquipment(UEquipment* Equipment)
{
	RelatedEquipment = Equipment;
}

void UInventoryBase::UnlinkEquipment()
{
	RelatedEquipment = nullptr;
}

UEquipment* UInventoryBase::GetEquipment() const
{
	return RelatedEquipment;
}

UItemBase* UInventoryBase::AddNewItem(UItemDataBase* ItemData, int32& OutOverflow, const int32 Amount)
{
	if (Amount <= 0)
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::AddNewItem: Cannot add Item, Amount is less than or equal to zero."));
		return nullptr;
	}
	
	OutOverflow = Amount;

	if (ItemData == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UInventoryBase::AddNewItem: Cannot add Item, ItemData is null."));
		return nullptr;
	}

	if (!CanSupportAdditionalWeight(ItemData->ItemWeight * Amount))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::AddNewItem: Cannot add item, overweight %s."), *GetName());
		return nullptr;
	}

	UItemBase* NewItem = nullptr;
	if (TryIncreaseAvailableStackForNewItem(ItemData, Amount, NewItem, OutOverflow))
		return NewItem;

	if (!CanContainItem(ItemData))
		return nullptr;

	UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::AddNewItem: Adding Item %s to inventory %s."), *ItemData->ItemTypeID, *GetName())
	int32 Overflow = 0;
	NewItem = UISFactory::CreateItems(this, ItemData, Overflow, Amount);
	OutOverflow = Overflow;

	if (NewItem == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UInventoryBase::AddNewItem: Failed to add Item %s to inventory %s"), *ItemData->ItemTypeID, *GetName());
		return nullptr;
	}

	AddWeight(NewItem->GetItemFullWeight());
	AddItemToList(NewItem);
	OnItemAdded(NewItem);

	if (Overflow > 0)
	{
		if (UItemBase* NewOverflowItem = AddNewItem(ItemData, OutOverflow, Overflow); NewOverflowItem != nullptr)
			return NewOverflowItem;
	}

	return NewItem;
}

bool UInventoryBase::TryAddItem(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UInventoryBase::TryAddItem: Cannot add Item, Item is null."));
		return false;
	}
	
	if (!CanSupportAdditionalWeight(Item->GetItemFullWeight()))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::TryAddItem: Cannot add item, overweight %s."), *GetName());
		return false;
	}

	if (TryIncreaseAvailableStackForItem(Item))
		return true;

	const UItemDataBase* ItemData = Item->GetItemData();
	if (!CanContainItem(ItemData))
		return false;

	AddWeight(Item->GetItemFullWeight());
	AddItemToList(Item);
	OnItemAdded(Item);
	return true;
}

bool UInventoryBase::RemoveItemByDataID(UItemDataBase* ItemData)
{
	if (UItemBase* FoundItem = Find(ItemData))
	{
		RemoveItem(FoundItem);
		return true;
	}

	return false;
}

void UInventoryBase::RemoveItem(UItemBase* Item)
{
	if (!IsValid(Item) || !Items.Contains(Item))
		return;

	RemoveWeight(Item->GetItemFullWeight());
	OnItemRemoved(Item);
	RemoveItemFromList(Item);
}

void UInventoryBase::ConsumeItem(UItemDataBase* ItemData, const int32 AmountToConsume, const bool bForceConsume) const
{
	if (!IsValid(ItemData) || AmountToConsume <= 0)
		return;

	auto ConsumeIteratively = [this](const UItemDataBase* ItemDataToFind, const int32 Amount)
	{
		int32 Remaining = Amount;

		while (Remaining > 0)
		{
			UItemBase* FoundItem = Find(ItemDataToFind);
			if (!IsValid(FoundItem))
				break;

			Remaining = FoundItem->Consume(Remaining);
		}
	};

	if (bForceConsume)
	{
		ConsumeIteratively(ItemData, AmountToConsume);
		return;
	}

	if (HasEnoughQuantityToConsume(ItemData, AmountToConsume))
		ConsumeIteratively(ItemData, AmountToConsume);
}

void UInventoryBase::ClearInventory()
{
	if (Items.Num() == 0 || IsEmpty()) // Items.Num == 0 is a safe check, sine IsEmpty() is a virtual function
		return;

	Items.Empty();
	OnClearedInventory();
	OnInventoryModified.Broadcast();
	OnInventoryCleared.Broadcast();
}

UItemBase* UInventoryBase::Find(const UItemDataBase* ItemData) const
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID)
			return Item;
	}

	return nullptr;
}

bool UInventoryBase::IsFull() const
{
	return Items.Num() >= Capacity;
}

bool UInventoryBase::IsCompletelyFull() const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->CanStack())
			return false;
	}

	return IsFull();
}

bool UInventoryBase::IsEmpty() const
{
	return Items.IsEmpty();
}

bool UInventoryBase::CanContainItem(const UItemDataBase* ItemData) const
{
	if (!UISInventoriesUtility::IsItemInRegistry(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::CanContainItem: Cannot add item, %s is not registered in the inventory registry %s."), *ItemData->GetName(), *GetName());
		return false;
	}

	if (ItemData->bIsUnique && HasItemOfTypeID(ItemData))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::CanContainItem: Cannot add item, ID %s is unique and already exists in the inventory %s."), *ItemData->ItemTypeID, *GetName());
		return false;
	}

	if (IsFull())
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::CanContainItem: Cannot add item, inventory %s is full."), *GetName());
		return false;
	}

	return true;
}

bool UInventoryBase::CanSupportAdditionalWeight(const float Weight) const
{
	if (!bUseWeight)
		return true;
	
	return CurrentWeight + Weight <= WeightMaxCapacity;
}

bool UInventoryBase::HasItemOfTypeID(const UItemDataBase* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemTypeID == ItemData->ItemTypeID)
			return true;
	}

	return false;
}

bool UInventoryBase::HasItemOfDataID(const UItemDataBase* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID)
			return true;
	}

	return false;
}

bool UInventoryBase::HasEnoughQuantityToConsume(const UItemDataBase* ItemData, const int32 QuantityToConsume) const
{
	if (!IsValid(ItemData))
		return false;

	if (QuantityToConsume <= 0)
		return true;

	int32 FoundQuantity = 0;

	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID)
			FoundQuantity += Item->GetCurrentQuantity();
	}

	return FoundQuantity >= QuantityToConsume;
}

UItemDataBase* UInventoryBase::GetItemDataFromRegistry(const FGuid ItemDataID) const
{
	return UISInventoriesUtility::GetItemDataFromRegistry(ItemDataID);
}

TArray<UItemBase*> UInventoryBase::GetItems(const bool bIncludeEquippedItems)
{
	TSet<UItemBase*> AllItems = TSet<UItemBase*>(Items);

	if (bIncludeEquippedItems && RelatedEquipment != nullptr)
		AllItems.Append(RelatedEquipment->GetEquippedItems());

	return AllItems.Array();
}

double UInventoryBase::GetCurrentWeight() const
{
	return CurrentWeight;
}

void UInventoryBase::BeginPlay()
{
	Super::BeginPlay();

	if (bIsMainInventory)
		UISInventoriesUtility::SetMainInventory(this);
}

USaveData* UInventoryBase::CreateSaveDataObject_Implementation()
{
	return NewObject<UInventoryBaseSaveData>();
}

USaveData* UInventoryBase::CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave)
{
	UInventoryBaseSaveData* InventorySaveData = Cast<UInventoryBaseSaveData>(SaveData);
	InventorySaveData->MaxWeight = WeightMaxCapacity;
	return SaveData;
}

void UInventoryBase::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	CurrentWeight = InventorySaveData->MaxWeight;
}

void UInventoryBase::AddItemToList(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UInventoryBase::AddItemToList: Cannot add Item, Item is null."));
		return;
	}
	
	Item->AssignInventory(this);
	Items.Add(Item);
	Item->OnAdd();
	Item->OnItemAdded.Broadcast(Item, this);
	OnAnyItemAdded.Broadcast(Item);
	OnInventoryModified.Broadcast();
}

void UInventoryBase::RemoveItemFromList(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UInventoryBase::RemoveItemFromList: Cannot remove Item, Item is null."));
		return;
	}
	
	OnAnyItemRemoved.Broadcast(Item);
	Item->OnRemove();
	Item->OnItemRemoved.Broadcast(Item, this);
	Items.Remove(Item);

	if (Item->RelatedInventory == this)
		Item->DeassignInventory();

	OnInventoryModified.Broadcast();
}

void UInventoryBase::OnItemAdded_Implementation(UItemBase* Item)
{
}

void UInventoryBase::OnItemRemoved_Implementation(UItemBase* Item)
{
}

void UInventoryBase::OnClearedInventory_Implementation()
{
}

bool UInventoryBase::TryIncreaseAvailableStackForNewItem(UItemDataBase* ItemData, const int32 Amount, UItemBase*& OutFoundItem, int32& OutOverflow)
{
	if (UItemBase* FoundItem = FindAvailableStackForNewItem(ItemData); IsValid(FoundItem))
	{
		const int32 Overflow = FoundItem->IncreaseQuantity(Amount);
		OutFoundItem = FoundItem;

		if (Overflow > 0)
			AddNewItem(ItemData, OutOverflow, Overflow);

		return true;
	}

	return false;
}

bool UInventoryBase::TryIncreaseAvailableStackForItem(UItemBase* Item) const
{
	if (UItemBase* FoundItem = FindAvailableStackForItem(Item); IsValid(FoundItem))
	{
		const int32 Amount = Item->GetCurrentQuantity();
		const int32 Rest = FoundItem->IncreaseQuantity(Amount);
		Item->SetQuantity(Rest);
		return true;
	}

	return false;
}

UItemBase* UInventoryBase::FindAvailableStackForNewItem(const UItemDataBase* ItemData) const
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID && Item->CanStack())
			return Item;
	}

	return nullptr;
}

UItemBase* UInventoryBase::FindAvailableStackForItem(UItemBase* Item) const
{
	for (UItemBase* StackableItem : Items)
	{
		if (StackableItem->CanStackItem(Item))
			return StackableItem;
	}

	return nullptr;
}

void UInventoryBase::AddWeight(const float Weight)
{
	if (!bUseWeight)
		return;
	
	CurrentWeight += Weight;
	CurrentWeight = FMath::Clamp(CurrentWeight, 0, WeightMaxCapacity);
}

void UInventoryBase::RemoveWeight(const float Weight)
{
	if (!bUseWeight)
		return;

	CurrentWeight -= Weight;
	CurrentWeight = FMath::Clamp(CurrentWeight, 0, WeightMaxCapacity);
}
