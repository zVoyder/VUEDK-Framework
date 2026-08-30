// Copyright VUEDK, Inc. All Rights Reserved.

#include "Subsystems/InventoryRegistrySubsystem.h"
#include "InventorySystem.h"
#include "InventorySystemSettings.h"
#include "Utility/ISInventoriesUtility.h"

void UInventoryRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UInventorySystemSettings* Settings = GetDefault<UInventorySystemSettings>())
	{
		for (const TSoftObjectPtr<UItemDataBase>& ItemData : Settings->RegisteredItems)
		{
			UItemDataBase* LoadedItemData = ItemData.LoadSynchronous();
			if (IsValid(LoadedItemData))
				RegisteredItems.Add(LoadedItemData->ItemDataId, LoadedItemData);
		}
	}

	UISInventoriesUtility::Init(this);
}

bool UInventoryRegistrySubsystem::IsItemInRegistry(const UItemDataBase* ItemData) const
{
	if (!IsValid(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UInventoryRegistrySubsystem::IsItemInRegistry: Invalid item data provided."));
		return false;
	}

	return RegisteredItems.Contains(ItemData->ItemDataId);
}

UItemDataBase* UInventoryRegistrySubsystem::GetItemDataFromRegistry(const FGuid& ItemDataId) const
{
	if (UItemDataBase* const* FoundItemData = RegisteredItems.Find(ItemDataId))
		return *FoundItemData;

	return nullptr;
}
