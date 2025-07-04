// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/ISInventoriesUtility.h"

#include "InventorySystemSettings.h"
#include "Base/InventoriesManager.h"
#include "Base/InventoryBase.h"
#include "Kismet/GameplayStatics.h"

UInventoriesManager* UISInventoriesUtility::InventoriesManager = nullptr;
UInventoryBase* UISInventoriesUtility::MainInventory = nullptr;
UInventoryBase* UISInventoriesUtility::TargetInventory = nullptr;

void UISInventoriesUtility::Init(UInventoriesManager* Manager)
{
	InventoriesManager = Manager;
}

void UISInventoriesUtility::SetMainInventory(UInventoryBase* Inventory)
{
	if (!Check())
		return;

	MainInventory = Inventory;
}

void UISInventoriesUtility::SetTargetInventory(UInventoryBase* Inventory)
{
	if (!Check())
		return;

	TargetInventory = Inventory;
}

UInventoryBase* UISInventoriesUtility::GetTargetInventory()
{
	return TargetInventory;
}

UInventoryBase* UISInventoriesUtility::GetMainInventory()
{
	return MainInventory;
}

UInventoriesManager* UISInventoriesUtility::GetInventoriesManager()
{
	if (!Check())
		return nullptr;

	return InventoriesManager;
}

UItemsRegistry* UISInventoriesUtility::GetRegistry()
{
	if (!Check()) return nullptr;

	return InventoriesManager->Registry;
}

bool UISInventoriesUtility::IsItemInRegistry(const UItemDataBase* ItemData)
{
	if (!Check()) return false;

	return InventoriesManager->IsItemInRegistry(ItemData);
}

UItemDataBase* UISInventoriesUtility::GetItemDataFromRegistry(const FGuid& ItemDataID)
{
	if (!Check()) return nullptr;

	return InventoriesManager->GetItemDataFromRegistry(ItemDataID);
}

bool UISInventoriesUtility::DropItemWithOperation(UItemBase* Item)
{
	const UInventorySystemSettings* Settings = GetDefault<UInventorySystemSettings>();

	if (!IsValid(Settings))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UISInventoriesUtility::DropItemWithOperation: UInventorySystemSettings is not valid!"));
		return false;
	}

	const TSubclassOf<UDropItemOperation> DropOperationClass = Settings->ItemsDropOperationClass.LoadSynchronous();

	if (!DropOperationClass)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UISInventoriesUtility::DropItemWithOperation: ItemsDropOperationClass is not set!"));
		return false;
	}

	UDropItemOperation* DropOperation = NewObject<UDropItemOperation>(GetTransientPackage(), DropOperationClass);

	if (!IsValid(DropOperation))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("UISInventoriesUtility::DropItemWithOperation: DropOperation is not valid!"));
		return false;
	}

	return DropOperation->DropItem(Item);
}

bool UISInventoriesUtility::Check()
{
	if (!IsValid(InventoriesManager))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("InventoriesManager is not valid!"));
		return false;
	}

	return true;
}
