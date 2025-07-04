// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGItemsGeneration/RPGItemStatOperation.h"
#include "RPGInventory/Data/RPGGearItemData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "StatsSystem/StatsBridgeBase.h"

void URPGItemStatOperation::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	if (RelatedItem = Cast<URPGGearItem>(Payload); RelatedItem == nullptr)
	{
		UE_LOG(LogStatsSystem, Error, TEXT("Failed to cast %s to URPGItem in %s"), *Payload->GetName(), *GetName());
		return;
	}

	GearBaseValue = RelatedItem->GetRPGGearItemData()->GearBaseMultiplier;
	RarityValue = RelatedItem->RarityLevel->RarityValue;
}
