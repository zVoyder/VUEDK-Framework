// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveGame/DefaultSaveGame.h"
#include "Data/SaveGame/SlotInfoItem.h"
#include "SaveManagerData.generated.h"

USTRUCT(BlueprintType)
struct FSaveManagerData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDefaultSaveGame> SaveGameClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDefaultSaveGame> SharedSaveGameClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlotInfoItem> SlotInfoItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlotInfoItem> SharedSlotInfoItemClass;

	FSaveManagerData() : SaveGameClass(USlotInfoItem::StaticClass()),
	                     SharedSaveGameClass(UDefaultSaveGame::StaticClass()),
	                     SlotInfoItemClass(USlotInfoItem::StaticClass()),
	                     SharedSlotInfoItemClass(USlotInfoItem::StaticClass())
	{
	}
};
