// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGItemsGeneration/RPGItemStatOperation.h"
#include "RPGSystem.h"
#include "RPGInventory/Data/RPGGearItemData.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "StatsSystem/StatsBridgeBase.h"

void URPGItemStatOperation::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	if (RelatedItem = Cast<URPGGearItem>(Payload); RelatedItem == nullptr)
		return;

	GearBaseValue = RelatedItem->GetRPGGearItemData()->GearBaseMultiplier;
	RarityValue = RelatedItem->RarityLevel->RarityValue;
}
