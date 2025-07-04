// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGInventory/Utility/RPGInventoriesUtility.h"
#include "Utility/ISInventoriesUtility.h"

URPGInventoryCompareHandlerSubsystem* URPGInventoriesUtility::CompareHandlerSubsystem = nullptr;

void URPGInventoriesUtility::Init(URPGInventoryCompareHandlerSubsystem* InCompareHandlerSubsystem)
{
	CompareHandlerSubsystem = InCompareHandlerSubsystem;
}

UCoreStatData* URPGInventoriesUtility::GetItemStatByID(const FGuid StatID)
{
	const URPGInventoriesManager* InventoriesManager = GetRPGInventoriesManager();

	if (!IsValid(InventoriesManager))
	{
		UE_LOG(LogTemp, Error, TEXT("URPGInventoriesUtility::GetRarityByID: RPGInventoriesManager is not valid!"));
		return nullptr;
	}

	return InventoriesManager->GetItemStatByID(StatID);
}

URPGRarityLevelData* URPGInventoriesUtility::GetItemRarityByID(const FGuid RarityID)
{
	const URPGInventoriesManager* InventoriesManager = GetRPGInventoriesManager();

	if (!IsValid(InventoriesManager))
	{
		UE_LOG(LogTemp, Error, TEXT("URPGInventoriesUtility::GetRarityByID: RPGInventoriesManager is not valid."));
		return nullptr;
	}

	return InventoriesManager->GetItemRarityByID(RarityID);
}

URPGInventoriesManager* URPGInventoriesUtility::GetRPGInventoriesManager()
{
	UInventoriesManager* InventoriesManager = UISInventoriesUtility::GetInventoriesManager();

	if (!IsValid(InventoriesManager))
		return nullptr;

	return Cast<URPGInventoriesManager>(InventoriesManager);
}

void URPGInventoriesUtility::SetTargetEquipmentForCompare(UEquipment* InComparingEquipment)
{
	if (!IsValid(CompareHandlerSubsystem))
		return;

	CompareHandlerSubsystem->SetTargetEquipmentForCompare(InComparingEquipment);
}

void URPGInventoriesUtility::SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey)
{
	if (!IsValid(CompareHandlerSubsystem))
		return;

	CompareHandlerSubsystem->SetTargetEquipSlotKeyForCompare(EquipSlotKey);
}

void URPGInventoriesUtility::RequestCompare(const bool bCompare)
{
	if (!IsValid(CompareHandlerSubsystem))
		return;

	CompareHandlerSubsystem->RequestCompare(bCompare);
}

bool URPGInventoriesUtility::IsComparing()
{
	if (!IsValid(CompareHandlerSubsystem))
		return false;

	return CompareHandlerSubsystem->IsComparing();
}

UEquipment* URPGInventoriesUtility::GetTargetComparingEquipment()
{
	if (!IsValid(CompareHandlerSubsystem))
		return nullptr;

	return CompareHandlerSubsystem->GetTargetComparingEquipment();
}

URPGGearItem* URPGInventoriesUtility::GetComparingItem()
{
	if (!IsValid(CompareHandlerSubsystem))
		return nullptr;

	return CompareHandlerSubsystem->GetComparingItem();
}

UEquipSlotKey* URPGInventoriesUtility::GetTargetComparingEquipSlotKey()
{
	if (!IsValid(CompareHandlerSubsystem))
		return nullptr;

	return CompareHandlerSubsystem->GetTargetComparingEquipSlotKey();
}
