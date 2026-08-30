// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/InventoryBase.h"
#include "InventorySystem.h"
#include "Base/ItemBase.h"
#include "Factories/ISFactory.h"
#include "GameFramework/PlayerState.h"
#include "Subsystems/InventoryTargetingSubsystem.h"
#include "Utility/ISInventoriesUtility.h"

UInventoryBase::UInventoryBase() : bUseWeight(false),
                                   WeightMaxCapacity(0),
                                   RelatedEquipment(nullptr),
                                   CurrentWeight(0)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryBase::LinkEquipment(UEquipment* Equipment)
{
	RelatedEquipment = Equipment;

	if (!IsValid(RelatedEquipment))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::LinkEquipment: Cannot link Equipment, Equipment is null."));
		return;
	}

	RelatedEquipment->OnEquipChanged.AddDynamic(this, &UInventoryBase::OnEquipmentChanged);
}

void UInventoryBase::UnlinkEquipment()
{
	if (IsValid(RelatedEquipment))
		RelatedEquipment->OnEquipChanged.RemoveDynamic(this, &UInventoryBase::OnEquipmentChanged);

	RelatedEquipment = nullptr;
}

UEquipment* UInventoryBase::GetEquipment() const
{
	return RelatedEquipment;
}

UItemBase* UInventoryBase::AddNewItem(UItemDataBase* ItemData, int32& OutOverflow, const int32 ItemQuantity)
{
	if (ItemQuantity <= 0)
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::AddNewItem: Cannot add Item, Amount is less than or equal to zero."));
		return nullptr;
	}

	OutOverflow = ItemQuantity;

	if (ItemData == nullptr)
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::AddNewItem: Cannot add Item, ItemData is null."));
		return nullptr;
	}

	if (!CanSupportAdditionalWeight(ItemData->ItemWeight * ItemQuantity))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::AddNewItem: Cannot add item, overweight %s."), *GetName());
		return nullptr;
	}

	UItemBase* NewItem = nullptr;
	if (TryIncreaseAvailableStackForNewItem(ItemData, ItemQuantity, NewItem, OutOverflow))
		return NewItem;

	if (!CanContainItem(ItemData))
		return nullptr;

	UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::AddNewItem: Adding Item %s to inventory %s."), *ItemData->ItemTypeId, *GetName())
	int32 Overflow = 0;
	NewItem = UISFactory::CreateItem(this, ItemData, Overflow, ItemQuantity);
	OutOverflow = Overflow;

	if (NewItem == nullptr)
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::AddNewItem: Failed to add Item %s to inventory %s"), *ItemData->ItemTypeId, *GetName());
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
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::TryAddItem: Cannot add Item, Item is null."));
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

bool UInventoryBase::RemoveItemByDataId(UItemDataBase* ItemData)
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

UItemBase* UInventoryBase::Find(const UItemDataBase* ItemData, const bool bSortByQuantity) const
{
	if (bSortByQuantity)
	{
		TArray<UItemBase*> FoundItems = FindAll(ItemData, true);
		if (FoundItems.Num() <= 0)
			return nullptr;

		return FoundItems[0];
	}

	if (!IsValid(ItemData))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::Find: ItemData is invalid, cannot find item in inventory %s."), *GetName());
		return nullptr;
	}

	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataId == ItemData->ItemDataId)
			return Item;
	}

	return nullptr;
}

TArray<UItemBase*> UInventoryBase::FindAll(const UItemDataBase* ItemData, const bool bSortByQuantity) const
{
	if (!IsValid(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::FindAll: ItemData is invalid, cannot find items in inventory %s."), *GetName());
		return TArray<UItemBase*>();
	}

	TArray<UItemBase*> FoundItems;

	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataId == ItemData->ItemDataId)
			FoundItems.Add(Item);
	}

	if (bSortByQuantity)
	{
		FoundItems.Sort([](const UItemBase& A, const UItemBase& B)
		{
			return A.GetCurrentQuantity() < B.GetCurrentQuantity();
		});
	}

	return FoundItems;
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

	if (ItemData->bIsUnique && HasItemOfTypeId(ItemData))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("UInventoryBase::CanContainItem: Cannot add item, Id %s is unique and already exists in the inventory %s."), *ItemData->ItemTypeId, *GetName());
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

bool UInventoryBase::HasItemOfTypeId(const UItemDataBase* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemTypeId == ItemData->ItemTypeId)
			return true;
	}

	return false;
}

bool UInventoryBase::HasItemOfDataId(const UItemDataBase* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataId == ItemData->ItemDataId)
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
		if (Item->GetItemData()->ItemDataId == ItemData->ItemDataId)
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
	SetupMainInventory();
}

void UInventoryBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(RelatedEquipment))
		UnlinkEquipment();
}

void UInventoryBase::AddItemToList(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::AddItemToList: Cannot add Item, Item is null."));
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
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::RemoveItemFromList: Cannot remove Item, Item is null."));
		return;
	}

	OnAnyItemRemoved.Broadcast(Item);
	Item->OnRemove();
	Item->OnItemRemoved.Broadcast(Item, this);
	Items.Remove(Item);

	if (Item->RelatedInventory == this)
		Item->ClearInventory();

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

void UInventoryBase::SetupMainInventory()
{
	if (!bIsMainInventory)
		return;
	
	if (const AActor* Owner = GetOwner(); IsValid(Owner))
	{
		const ULocalPlayer* LocalPlayer = nullptr;
		
		if (const APlayerState* PlayerState = Cast<APlayerState>(Owner); IsValid(PlayerState))
		{
			if (const APlayerController* PlayerController = PlayerState->GetPlayerController(); IsValid(PlayerController))
				LocalPlayer = PlayerController->GetLocalPlayer();
		}
		else if (const APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController()); IsValid(PlayerController))
		{
			LocalPlayer = PlayerController->GetLocalPlayer();
		}
		else if (const APawn* Pawn = Cast<APawn>(Owner); IsValid(Pawn))
		{
			if (const APlayerController* PawnPlayerController = Pawn->GetLocalViewingPlayerController(); IsValid(PawnPlayerController))
				LocalPlayer = PawnPlayerController->GetLocalPlayer();
		}

		if (!IsValid(LocalPlayer))
			return;
		
		UInventoryTargetingSubsystem* TargetingSubsystem = LocalPlayer->GetSubsystem<UInventoryTargetingSubsystem>();
		TargetingSubsystem->SetMainInventory(this);
	}
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
		if (Item->GetItemData()->ItemDataId == ItemData->ItemDataId && Item->CanStack())
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

void UInventoryBase::OnEquipmentChanged()
{
	for (UItemBase* Item : RelatedEquipment->GetEquippedItems())
	{
		if (Item->IsEquipped())
		{
			if (!Item->CanEquip())
			{
				if (!TryAddItem(Item))
				{
					const UWorld* World = GetWorld();
					if (!IsValid(World))
						continue;

					APlayerController* PlayerController = World->GetFirstPlayerController();
					if (!UISInventoriesUtility::DropItemWithOperation(PlayerController, Item))
						UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryBase::OnEquipmentChanged: Failed to drop item %s."), *Item->GetName());
				}
			}
		}
	}
}
