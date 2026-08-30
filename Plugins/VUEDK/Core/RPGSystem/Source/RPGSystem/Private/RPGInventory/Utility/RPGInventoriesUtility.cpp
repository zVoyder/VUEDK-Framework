// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Utility/RPGInventoriesUtility.h"
#include "Utility/ISInventoriesUtility.h"

TWeakObjectPtr<URPGInventoryCompareHandlerSubsystem> URPGInventoriesUtility::CompareHandlerSubsystem = nullptr;
TWeakObjectPtr<URPGRegistrySubsystem> URPGInventoriesUtility::RPGRegistrySubsystem = nullptr;

void URPGInventoriesUtility::Init(URPGRegistrySubsystem* InRPGRegistrySubsystem)
{
	RPGRegistrySubsystem = InRPGRegistrySubsystem;
}

void URPGInventoriesUtility::SetCompareHandler(URPGInventoryCompareHandlerSubsystem* InCompareHandlerSubsystem)
{
	CompareHandlerSubsystem = InCompareHandlerSubsystem;
}

UCoreStatData* URPGInventoriesUtility::GetItemStatByID(const FGuid StatID)
{
	if (!RPGRegistrySubsystem.IsValid())
		return nullptr;
	
	return RPGRegistrySubsystem.Get()->GetItemStatById(StatID);
}

URPGRarityLevelData* URPGInventoriesUtility::GetItemRarityByID(const FGuid RarityID)
{
	if (!RPGRegistrySubsystem.IsValid())
		return nullptr;
	
	return RPGRegistrySubsystem.Get()->GetItemRarityById(RarityID);
}

void URPGInventoriesUtility::SetTargetEquipmentForCompare(UEquipment* InComparingEquipment)
{
	if (!CompareHandlerSubsystem.IsValid())
		return;

	CompareHandlerSubsystem.Get()->SetTargetEquipmentForCompare(InComparingEquipment);
}

void URPGInventoriesUtility::SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey)
{
	if (!CompareHandlerSubsystem.IsValid())
		return;

	CompareHandlerSubsystem.Get()->SetTargetEquipSlotKeyForCompare(EquipSlotKey);
}

void URPGInventoriesUtility::RequestCompare(const bool bCompare)
{
	if (!CompareHandlerSubsystem.IsValid())
		return;

	CompareHandlerSubsystem.Get()->RequestCompare(bCompare);
}

bool URPGInventoriesUtility::IsComparing()
{
	if (!CompareHandlerSubsystem.IsValid())
		return false;

	return CompareHandlerSubsystem.Get()->IsComparing();
}

UEquipment* URPGInventoriesUtility::GetTargetComparingEquipment()
{
	if (!CompareHandlerSubsystem.IsValid())
		return nullptr;

	return CompareHandlerSubsystem.Get()->GetTargetComparingEquipment();
}

URPGGearItem* URPGInventoriesUtility::GetComparingItem()
{
	if (!CompareHandlerSubsystem.IsValid())
		return nullptr;

	return CompareHandlerSubsystem.Get()->GetComparingItem();
}

UEquipSlotKey* URPGInventoriesUtility::GetTargetComparingEquipSlotKey()
{
	if (!CompareHandlerSubsystem.IsValid())
		return nullptr;

	return CompareHandlerSubsystem.Get()->GetTargetComparingEquipSlotKey();
}
