// Copyright VUEDK, Inc. All Rights Reserved.

#include "Subsystems/InventoryTargetingSubsystem.h"

void UInventoryTargetingSubsystem::SetMainInventory(UInventoryBase* Inventory)
{
	MainInventory = Inventory;
}

void UInventoryTargetingSubsystem::SetTargetInventory(UInventoryBase* Inventory)
{
	TargetInventory = Inventory;
}

UInventoryBase* UInventoryTargetingSubsystem::GetMainInventory() const
{
	return MainInventory.Get();
}

UInventoryBase* UInventoryTargetingSubsystem::GetTargetInventory() const
{
	return TargetInventory.Get();
}
