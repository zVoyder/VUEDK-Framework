// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/ISInventoriesUtility.h"
#include "InventorySystem.h"
#include "InventorySystemSettings.h"
#include "Factories/ISFactory.h"
#include "Subsystems/InventoryRegistrySubsystem.h"

TWeakObjectPtr<UInventoryRegistrySubsystem> UISInventoriesUtility::InventoryRegistrySubsystem = nullptr;

void UISInventoriesUtility::Init(UInventoryRegistrySubsystem* InInventoryRegistrySubsystem)
{
	InventoryRegistrySubsystem = InInventoryRegistrySubsystem;
}

bool UISInventoriesUtility::IsItemInRegistry(const UItemDataBase* ItemData)
{
	if (!Check())
		return false;

	return InventoryRegistrySubsystem.Get()->IsItemInRegistry(ItemData);
}

UItemDataBase* UISInventoriesUtility::GetItemDataFromRegistry(const FGuid& ItemDataId)
{
	if (!Check())
		return nullptr;

	return InventoryRegistrySubsystem.Get()->GetItemDataFromRegistry(ItemDataId);
}

bool UISInventoriesUtility::DropItemWithOperation(APlayerController* Instigator, UItemBase* Item)
{
	const UInventorySystemSettings* Settings = GetDefault<UInventorySystemSettings>();

	if (!IsValid(Settings))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISInventoriesUtility::DropItemWithOperation: UInventorySystemSettings is not valid."));
		return false;
	}

	const TSubclassOf<UDropItemOperation> DropOperationClass = Settings->ItemsDropOperationClass.LoadSynchronous();
	UDropItemOperation* DropOperation = UISFactory::CreateItemDropOperation(Instigator, DropOperationClass);
	if (!IsValid(DropOperation))
		return false;

	return DropOperation->DropItem(Item);
}

bool UISInventoriesUtility::Check()
{
	return InventoryRegistrySubsystem.IsValid();
}
