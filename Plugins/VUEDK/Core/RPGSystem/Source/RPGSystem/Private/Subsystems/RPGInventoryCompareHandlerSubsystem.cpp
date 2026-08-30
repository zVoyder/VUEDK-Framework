// Copyright VUEDK, Inc. All Rights Reserved.

#include "Subsystems/RPGInventoryCompareHandlerSubsystem.h"
#include "RPGSystem.h"
#include "RPGInventory/Utility/RPGInventoriesUtility.h"

void URPGInventoryCompareHandlerSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);
	URPGInventoriesUtility::SetCompareHandler(this);
}

void URPGInventoryCompareHandlerSubsystem::SetTargetEquipmentForCompare(UEquipment* InComparingEquipment)
{
	if (!IsValid(InComparingEquipment))
	{
		UE_LOG(LogRPGSystem, Warning, TEXT("URPGInventoryCompareHandlerSubsystem::SetTargetEquipmentForCompare: Invalid Equipment."));
		return;
	}
	
	TargetComparingEquipment = InComparingEquipment;
}

void URPGInventoryCompareHandlerSubsystem::SetTargetEquipSlotKeyForCompare(UEquipSlotKey* EquipSlotKey)
{
	TargetEquipSlotKey = EquipSlotKey;
}

void URPGInventoryCompareHandlerSubsystem::RequestCompare(const bool bCompare)
{
	bIsComparing = bCompare;

	if (!IsValid(TargetComparingEquipment) || !IsValid(TargetEquipSlotKey))
		return;
	
	OnRequestCompare.Broadcast(GetComparingItem(), bCompare);
}

bool URPGInventoryCompareHandlerSubsystem::IsComparing() const
{
	return bIsComparing;
}

UEquipment* URPGInventoryCompareHandlerSubsystem::GetTargetComparingEquipment() const
{
	return TargetComparingEquipment;
}

URPGGearItem* URPGInventoryCompareHandlerSubsystem::GetComparingItem()
{
	if (!IsValid(TargetComparingEquipment) || !IsValid(TargetEquipSlotKey))
		return nullptr;
	
	TargetComparingItem = Cast<URPGGearItem>(TargetComparingEquipment->FindEquippedItem(TargetEquipSlotKey));
	return TargetComparingItem;
}

UEquipSlotKey* URPGInventoryCompareHandlerSubsystem::GetTargetComparingEquipSlotKey() const
{
	return TargetEquipSlotKey;
}
