// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/InventoriesManager.h"
#include "Base/InventoryBase.h"
#include "Utility/ISInventoriesUtility.h"

UInventoriesManager::UInventoriesManager(): Registry(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoriesManager::IsItemInRegistry(const UItemDataBase* ItemData) const
{
	if (!Check())
		return false;
	
	return Registry->RegisteredItems.Contains(ItemData);
}

UItemDataBase* UInventoriesManager::GetItemDataFromRegistry(const FGuid& ItemDataID) const
{
	if (!Check())
		return nullptr;

	if (Registry->RegisteredItems.Num() == 0)
		return nullptr;
	
	for (UItemDataBase* ItemData : Registry->RegisteredItems)
	{
		if (!IsValid(ItemData))
		{
			UE_LOG(LogInventorySystem, Error, TEXT("GetItemDataFromRegistry: Found invalid item data in registry."));
			continue;
		}

		if (ItemData->ItemDataID == ItemDataID)
			return ItemData;
	}

	return nullptr;
}

void UInventoriesManager::BeginPlay()
{
	UISInventoriesUtility::Init(this);
	Super::BeginPlay();
}

bool UInventoriesManager::Check() const
{
	const bool bIsValid = Registry != nullptr;

	if (!bIsValid)
		UE_LOG(LogInventorySystem, Error, TEXT("Inventory registry is not valid!"));

	return bIsValid;
}
