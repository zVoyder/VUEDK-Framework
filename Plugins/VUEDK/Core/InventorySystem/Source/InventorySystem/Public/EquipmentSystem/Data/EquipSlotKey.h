// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipSlotKey.generated.h"

UCLASS(BlueprintType, NotBlueprintable)
class INVENTORYSYSTEM_API UEquipSlotKey : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipSlotKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EquipSlotName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush EquipSlotBrush;

public:
	UEquipSlotKey() : EquipSlotKey(FName("NotGeneratedID")),
	                  EquipSlotName(FText::FromString("Equip Slot Name"))
	{
	}

	FORCEINLINE virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		EquipSlotKey = FName(FGuid::NewGuid().ToString());
	}
};
